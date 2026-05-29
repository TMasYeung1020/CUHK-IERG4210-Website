const express = require('express');
const sqlite3 = require('sqlite3').verbose();
const path = require('path');
const session = require('express-session');
const crypto = require('crypto');
const multer = require('multer');

const app = express();
const port = 3000;
const dbPath = path.resolve(__dirname, 'database', 'shop.db');
const db = new sqlite3.Database(dbPath);

// 請將此處替換為你的 Stripe Secret Key (sk_test_...)
const stripe = require('stripe')('sk_test_51TVYrcK5GJgT8d1EZIjb0Ci7SfRFxbxGPMXJfo5dN7TOnfiyg5PucQ3JQPy7D1eNMnjUvLnztvBgl5FJ3qmky52o00FQQmxfUo');
const MERCHANT_EMAIL = "1155203181@link.cuhk.edu.hk";

// ==========================================
// 1. Stripe Webhook (必須放在 express.json() 之前)
// ==========================================
app.post('/webhook', express.raw({type: 'application/json'}), async (req, res) => {
    const sig = req.headers['stripe-signature'];
    let event;

    try {
        // 請將此處替換為你的 Webhook Signing Secret (whsec_...)
        event = stripe.webhooks.constructEvent(req.body, sig, 'whsec_0be30f6c73cc448e17ba76a4205a4f43c45fdb29904571210f3ff85b7fd5900b');
    } catch (err) {
        console.error(`Webhook Error: ${err.message}`);
        return res.status(400).send(`Webhook Error: ${err.message}`);
    }

    if (event.type === 'checkout.session.completed') {
        const session = event.data.object;
        const orderId = session.metadata.orderId;

        // 從資料庫抓取原始訂單數據來驗證
        db.get("SELECT * FROM orders WHERE oid = ?", [orderId], (err, order) => {
            if (order) {
                // 重新組合原始數據生成 Digest 以驗證資料完整性
                const digestSource = [
                    order.currency, 
                    MERCHANT_EMAIL, 
                    order.salt, 
                    order.items_json, 
                    order.total_price
                ].join('|');
                
                const calculatedDigest = crypto.createHash('sha256').update(digestSource).digest('hex');

                // 比對 Digest
                if (calculatedDigest === order.digest) {
                    db.run("UPDATE orders SET status = 'PAID', stripe_session_id = ? WHERE oid = ?", 
                        [session.id, orderId], (err) => {
                            if (!err) console.log(`✅ 訂單 ${orderId} 驗證成功並已更新為 PAID`);
                        });
                } else {
                    console.error(`❌ 訂單 ${orderId} 摘要不符，可能遭到篡改！`);
                }
            }
        });
    }
    res.json({received: true});
});

// ==========================================
// 2. 標準 Middleware
// ==========================================
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// --- Phase 4: Session 配置 ---
app.use(session({
    secret: 'ierg4210-cuhk-secret-2026',
    resave: false,
    saveUninitialized: false,
    name: 'auth_token',
    cookie: { 
        httpOnly: true, 
        sameSite: 'lax',
        secure: false,    // localhost 環境設為 false
        maxAge: 24 * 60 * 60 * 1000 // 1天 (符合 0 < expires < 3 days)
    }
}));

// --- Phase 4: Security Headers (CSP) ---
app.use((req, res, next) => {
    // 修正後
res.setHeader("Content-Security-Policy", "default-src 'self'; script-src 'self' 'unsafe-inline' https://js.stripe.com; style-src 'self' 'unsafe-inline'; img-src 'self' data: https://" + "*.stripe.com; frame-src https://js.stripe.com;");
    next();
});

// --- Phase 4: Multer 配置 ---
const storage = multer.diskStorage({
    destination: '/var/www/html/uploads/',
    filename: (req, file, cb) => cb(null, Date.now() + path.extname(file.originalname))
});
const upload = multer({ storage: storage, limits: { fileSize: 10000000 } });

// --- Security Helpers ---
function verifyPassword(input, hash, salt) {
    return crypto.pbkdf2Sync(input, salt, 1000, 64, 'sha512').toString('hex') === hash;
}

// server.js 
function getDiscountedPrice(qty, originalPrice, discountRule) {
    if (!discountRule || qty < discountRule.threshold) {
        return qty * originalPrice;
    }

    if (discountRule.type === 'B2G1') {
        // 買二送一邏輯：每達到 threshold (3)，就有一個免費
        const freeItems = Math.floor(qty / discountRule.threshold);
        return (qty - freeItems) * originalPrice;
    } 
    else if (discountRule.type === 'BULK') {
        // 多買多平邏輯：每 threshold (2) 個一組，一組收 discount_val
        const sets = Math.floor(qty / discountRule.threshold);
        const remainder = qty % discountRule.threshold;
        return (sets * discountRule.discount_val) + (remainder * originalPrice);
    }
    return qty * originalPrice;
}

function checkAuth(req, res, next) {
    if (req.session && req.session.user && req.session.user.isAdmin === 1) {
        return next();
    }
    if (req.session && req.session.user) {
        return res.status(403).json({ success: false, message: "存取被拒：需要管理員權限" });
    }
    res.status(401).json({ success: false, message: "請先登入" });
}

function checkCSRF(req, res, next) {
    const clientToken = req.headers['x-csrf-token'] || req.body.nonce;
    if (req.session.csrfToken && clientToken === req.session.csrfToken) return next();
    res.status(403).json({ success: false, message: "Invalid CSRF Token" });
}

// ==========================================
// 3. API Endpoints
// ==========================================

app.get('/api/categories', (req, res) => {
    db.all("SELECT * FROM categories", (err, rows) => res.json(rows));
});

app.get('/api/products/:catid', (req, res) => {
    db.all("SELECT * FROM products WHERE catid = ?", [req.params.catid], (err, rows) => res.json(rows));
});

app.get('/api/product/:pid', (req, res) => {
    db.get("SELECT * FROM products WHERE pid = ?", [req.params.pid], (err, row) => {
        if (err) return res.status(500).json({ error: err.message });
        if (!row) return res.status(404).json({ message: "Product not found" });
        res.json(row);
    });
});

app.get('/api/admin/orders', checkAuth, (req, res) => {
    db.all("SELECT * FROM orders ORDER BY created_at DESC", (err, rows) => {
        if (err) return res.status(500).json({ success: false });
        res.json(rows);
    });
});

app.get('/api/user/recent-orders', (req, res) => {
    if (!req.session.user) return res.status(401).send();
    const userid = req.session.user.id;
    db.all("SELECT * FROM orders WHERE userid = ? ORDER BY created_at DESC LIMIT 5", [userid], (err, rows) => {
        res.json(rows);
    });
});

app.post('/api/login', (req, res) => {
    const { email, password } = req.body;
    db.get("SELECT * FROM users WHERE email = ?", [email], (err, user) => {
        if (user && verifyPassword(password, user.password, user.salt)) {
            req.session.regenerate(() => {
                req.session.user = { id: user.userid, email: user.email, isAdmin: user.isAdmin };
                req.session.csrfToken = crypto.randomBytes(16).toString('hex');
                res.json({ 
                    success: true, 
                    csrfToken: req.session.csrfToken,
                    isAdmin: user.isAdmin 
                });
            });
        } else {
            res.status(401).json({ success: false, message: "Login failed" });
        }
    });
});

app.post('/api/register', (req, res) => {
    const { email, password, confirm } = req.body;
    if (!email || !password || !confirm) {
        return res.status(400).json({ success: false, message: "Missing fields" });
    }
    if (password !== confirm) {
        return res.status(400).json({ success: false, message: "Passwords do not match" });
    }
    db.get("SELECT email FROM users WHERE email = ?", [email], (err, row) => {
        if (row) return res.status(400).json({ success: false, message: "Email already registered" });
        const salt = crypto.randomBytes(16).toString('hex');
        const hash = crypto.pbkdf2Sync(password, salt, 1000, 64, 'sha512').toString('hex');
        db.run("INSERT INTO users (email, password, salt, isAdmin) VALUES (?, ?, ?, 0)", 
            [email, hash, salt], (err) => {
                if (err) return res.status(500).json({ success: false, message: "Database error" });
                res.json({ success: true, message: "User registered" });
            });
    });
});

app.post('/api/logout', (req, res) => {
    req.session.destroy(() => {
        res.clearCookie('auth_token');
        res.json({ success: true });
    });
});

app.post('/api/admin/product', checkAuth, checkCSRF, upload.single('image'), (req, res) => {
    const { catid, name, price, description } = req.body;
    db.run("INSERT INTO products (catid, name, price, description, image) VALUES (?,?,?,?,?)",
        [catid, name, price, description, req.file.filename], (err) => {
            if (err) return res.status(500).json({ success: false, message: err.message });
            res.json({ success: true });
        });
});

app.post('/api/admin/change-password', checkAuth, checkCSRF, (req, res) => {
    const { oldPassword, newPassword } = req.body;
    const userId = req.session.user.id;
    db.get("SELECT * FROM users WHERE userid = ?", [userId], (err, user) => {
        if (!user) return res.status(404).json({ success: false, message: "User not found" });
        if (!verifyPassword(oldPassword, user.password, user.salt)) {
            return res.status(400).json({ success: false, message: "Current password incorrect" });
        }
        const newSalt = crypto.randomBytes(16).toString('hex');
        const newHash = crypto.pbkdf2Sync(newPassword, newSalt, 1000, 64, 'sha512').toString('hex');
        db.run("UPDATE users SET password = ?, salt = ? WHERE userid = ?", 
            [newHash, newSalt, userId], (err) => {
                if (err) return res.status(500).json({ success: false, message: "Update failed" });
                req.session.destroy(() => {
                    res.clearCookie('auth_token');
                    res.json({ success: true, message: "Password updated, please login again" });
                });
            });
    });
});

// [Phase 6 - 動態折扣引擎結帳邏輯]
app.post('/api/orders/create', async (req, res) => {
    if (!req.session.user) return res.status(401).json({ success: false, message: "Please login" });

    const { items } = req.body; // 預期格式: [{pid: 1, quantity: 3}, ...]
    const userid = req.session.user.id;
    const currency = 'hkd';
    const salt = crypto.randomBytes(16).toString('hex');

    try {
        let totalHKD = 0;
        let validatedItems = [];
        let lineItemsForStripe = [];

        for (let item of items) {
            // 使用 LEFT JOIN 一次過抓取產品與折扣規則
            const row = await new Promise((resolve, reject) => {
                const sql = `
                    SELECT p.pid, p.name, p.price, d.type, d.threshold, d.discount_val 
                    FROM products p 
                    LEFT JOIN discounts d ON p.pid = d.pid 
                    WHERE p.pid = ?`;
                db.get(sql, [item.pid], (err, row) => err ? reject(err) : resolve(row));
            });

            if (row && item.quantity > 0) {
                const discountRule = row.type ? { type: row.type, threshold: row.threshold, discount_val: row.discount_val } : null;
                
                // 調用你定義好的 getDiscountedPrice 函數
                const finalPriceForThisItem = getDiscountedPrice(item.quantity, row.price, discountRule);
                totalHKD += finalPriceForThisItem;

                validatedItems.push({
                    pid: row.pid,
                    name: row.name,
                    quantity: item.quantity,
                    total_item_price: finalPriceForThisItem // 這是折扣後的該項總價
                });

                // 準備 Stripe 顯示資料 (Stripe 需要整數分 Cents)
                lineItemsForStripe.push({
                    price_data: {
                        currency: currency,
                        product_data: { 
                            name: row.name + (row.type && item.quantity >= row.threshold ? " (Promotion Applied)" : "") 
                        },
                        unit_amount: Math.round((finalPriceForThisItem / item.quantity) * 100),
                    },
                    quantity: item.quantity,
                });
            }
        }

        // 重新生成 Digest 確保安全性 (包含折扣後的價格)
        const itemsJson = JSON.stringify(validatedItems);
        const digestSource = [currency, MERCHANT_EMAIL, salt, itemsJson, totalHKD].join('|');
        const digest = crypto.createHash('sha256').update(digestSource).digest('hex');

        // 寫入資料庫
        db.run(`INSERT INTO orders (userid, total_price, currency, items_json, salt, digest, status) VALUES (?, ?, ?, ?, ?, ?, 'PENDING')`,
            [userid, totalHKD, currency, itemsJson, salt, digest], 
            async function(err) {
                if (err) {
                    console.error("DB Insert Error:", err);
                    return res.status(500).json({ success: false });
                }

                const orderId = this.lastID;

                // 建立 Stripe 會話
                const session = await stripe.checkout.sessions.create({
                    payment_method_types: ['card'],
                    line_items: lineItemsForStripe,
                    mode: 'payment',
                    success_url: `http://s89.ierg4210.iecuhk.cc/checkout-success.html?oid=${orderId}`,
                    cancel_url: `http://s89.ierg4210.iecuhk.cc/checkout-cancel.html`,
                    metadata: { orderId: orderId.toString() }
                });

                res.json({ success: true, url: session.url });
            }
        );
    } catch (e) {
        console.error("Checkout logic error:", e);
        res.status(500).json({ error: e.message });
    }
});

// [Phase 6 - Requirement 10] 獲取當前登入用戶的訂單
app.get('/api/user/orders', (req, res) => {
    if (!req.session || !req.session.user) {
        return res.status(401).json({ success: false, message: "Unauthorized" });
    }

    // 修正：這裡應該使用 id，因為你在登入時存的是 id (見第 135 行)
    const userId = req.session.user.id;

    // 修正：確保資料表欄位名稱與你資料庫一致（通常是 userid 或 uid）
    // 這裡我們假設你的 orders 表格欄位叫 userid (見第 210 行)
    db.all("SELECT oid, total_price, status FROM orders WHERE userid = ? ORDER BY oid DESC", [userId], (err, rows) => {
        if (err) {
            console.error("Fetch orders error:", err);
            return res.status(500).json({ success: false, message: "Database error" });
        }
        res.json({ success: true, orders: rows });
    });
});

app.get('/api/discounts/:pid', (req, res) => {
    db.get("SELECT * FROM discounts WHERE pid = ?", [req.params.pid], (err, row) => {
        res.json(row || null);
    });
});

// ==========================================
// 4. 路由與靜態服務
// ==========================================

app.get('/admin/index.html', (req, res, next) => {
    if (req.session && req.session.user && req.session.user.isAdmin === 1) {
        return next();
    }
    res.redirect(req.session?.user ? '/index.html' : '/login.html');
});

app.get('/admin/', (req, res) => res.redirect('/admin/index.html'));

app.use(express.static('public'));

// 錯誤處理 (捕捉異步報錯)
process.on('unhandledRejection', (reason) => {
    console.error('Unhandled Rejection:', reason);
});

app.listen(port, () => console.log(`🚀 Server running at http://localhost:${port}`));