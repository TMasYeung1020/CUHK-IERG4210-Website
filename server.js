const express = require('express');
const sqlite3 = require('sqlite3').verbose();
const path = require('path');
const fs = require('fs');
const session = require('express-session');
const crypto = require('crypto');
const multer = require('multer');
const { seedIfNeeded } = require('./init');

const app = express();
const port = process.env.PORT || 3000;
const dbPath = path.resolve(__dirname, 'database', 'shop.db');
fs.mkdirSync(path.dirname(dbPath), { recursive: true });
const db = new sqlite3.Database(dbPath);

const stripe = require('stripe')(process.env.STRIPE_SECRET_KEY);
const MERCHANT_EMAIL = "1155203181@link.cuhk.edu.hk";
const HOST_URL = (process.env.HOST_URL || 'http://localhost:3000').replace(/\/$/, '');

// ==========================================
// 1. Stripe Webhook (must come before express.json())
// ==========================================
app.post('/webhook', express.raw({type: 'application/json'}), async (req, res) => {
    const sig = req.headers['stripe-signature'];
    let event;

    try {
        event = stripe.webhooks.constructEvent(req.body, sig, process.env.STRIPE_WEBHOOK_SECRET);
    } catch (err) {
        console.error(`Webhook Error: ${err.message}`);
        return res.status(400).send(`Webhook Error: ${err.message}`);
    }

    if (event.type === 'checkout.session.completed') {
        const session = event.data.object;
        const orderId = session.metadata.orderId;

        db.get("SELECT * FROM orders WHERE oid = ?", [orderId], (err, order) => {
            if (order) {
                const digestSource = [
                    order.currency,
                    MERCHANT_EMAIL,
                    order.salt,
                    order.items_json,
                    order.total_price
                ].join('|');

                const calculatedDigest = crypto.createHash('sha256').update(digestSource).digest('hex');

                if (calculatedDigest === order.digest) {
                    db.run("UPDATE orders SET status = 'PAID', stripe_session_id = ? WHERE oid = ?",
                        [session.id, orderId], (err) => {
                            if (!err) console.log(`✅ Order ${orderId} verified and marked PAID`);
                        });
                } else {
                    console.error(`❌ Order ${orderId} digest mismatch — possible tampering`);
                }
            }
        });
    }
    res.json({received: true});
});

// ==========================================
// 2. Middleware
// ==========================================
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.use(session({
    secret: process.env.SESSION_SECRET || 'dev-fallback-secret',
    resave: false,
    saveUninitialized: false,
    name: 'auth_token',
    cookie: {
        httpOnly: true,
        sameSite: 'lax',
        secure: process.env.NODE_ENV === 'production',
        maxAge: 24 * 60 * 60 * 1000
    }
}));

app.use((req, res, next) => {
    res.setHeader("Content-Security-Policy", "default-src 'self'; script-src 'self' 'unsafe-inline' https://js.stripe.com; style-src 'self' 'unsafe-inline'; img-src 'self' data: https://*.stripe.com; frame-src https://js.stripe.com;");
    next();
});

const uploadsDir = path.join(__dirname, 'public', 'uploads');
fs.mkdirSync(uploadsDir, { recursive: true });
const storage = multer.diskStorage({
    destination: uploadsDir,
    filename: (req, file, cb) => cb(null, Date.now() + path.extname(file.originalname))
});
const upload = multer({ storage: storage, limits: { fileSize: 10000000 } });

// ==========================================
// 3. Helpers
// ==========================================
function verifyPassword(input, hash, salt) {
    return crypto.pbkdf2Sync(input, salt, 1000, 64, 'sha512').toString('hex') === hash;
}

function getDiscountedPrice(qty, originalPrice, discountRule) {
    if (!discountRule || qty < discountRule.threshold) {
        return qty * originalPrice;
    }
    if (discountRule.type === 'B2G1') {
        const freeItems = Math.floor(qty / discountRule.threshold);
        return (qty - freeItems) * originalPrice;
    }
    if (discountRule.type === 'BULK') {
        const sets = Math.floor(qty / discountRule.threshold);
        const remainder = qty % discountRule.threshold;
        return (sets * discountRule.discount_val) + (remainder * originalPrice);
    }
    return qty * originalPrice;
}

function checkAuth(req, res, next) {
    if (req.session && req.session.user && req.session.user.isAdmin === 1) return next();
    if (req.session && req.session.user) {
        return res.status(403).json({ success: false, message: "Admin access required" });
    }
    res.status(401).json({ success: false, message: "Please login" });
}

function checkCSRF(req, res, next) {
    const clientToken = req.headers['x-csrf-token'] || req.body.nonce;
    if (req.session.csrfToken && clientToken === req.session.csrfToken) return next();
    res.status(403).json({ success: false, message: "Invalid CSRF Token" });
}

// ==========================================
// 4. API Endpoints
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
                res.json({ success: true, csrfToken: req.session.csrfToken, isAdmin: user.isAdmin });
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

app.post('/api/orders/create', async (req, res) => {
    if (!req.session.user) return res.status(401).json({ success: false, message: "Please login" });

    const { items } = req.body;
    const userid = req.session.user.id;
    const currency = 'hkd';
    const salt = crypto.randomBytes(16).toString('hex');

    try {
        let totalHKD = 0;
        let validatedItems = [];
        let lineItemsForStripe = [];

        for (let item of items) {
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
                const finalPriceForThisItem = getDiscountedPrice(item.quantity, row.price, discountRule);
                totalHKD += finalPriceForThisItem;

                validatedItems.push({
                    pid: row.pid,
                    name: row.name,
                    quantity: item.quantity,
                    total_item_price: finalPriceForThisItem
                });

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

        const itemsJson = JSON.stringify(validatedItems);
        const digestSource = [currency, MERCHANT_EMAIL, salt, itemsJson, totalHKD].join('|');
        const digest = crypto.createHash('sha256').update(digestSource).digest('hex');

        db.run(`INSERT INTO orders (userid, total_price, currency, items_json, salt, digest, status) VALUES (?, ?, ?, ?, ?, ?, 'PENDING')`,
            [userid, totalHKD, currency, itemsJson, salt, digest],
            async function(err) {
                if (err) {
                    console.error("DB Insert Error:", err);
                    return res.status(500).json({ success: false });
                }

                const orderId = this.lastID;

                const session = await stripe.checkout.sessions.create({
                    payment_method_types: ['card'],
                    line_items: lineItemsForStripe,
                    mode: 'payment',
                    success_url: `${HOST_URL}/checkout-success.html?oid=${orderId}`,
                    cancel_url: `${HOST_URL}/checkout-cancel.html`,
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

app.get('/api/user/orders', (req, res) => {
    if (!req.session || !req.session.user) {
        return res.status(401).json({ success: false, message: "Unauthorized" });
    }
    const userId = req.session.user.id;
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
// 5. Static routes
// ==========================================

app.get('/admin/index.html', (req, res, next) => {
    if (req.session && req.session.user && req.session.user.isAdmin === 1) return next();
    res.redirect(req.session?.user ? '/index.html' : '/login.html');
});

app.get('/admin/', (req, res) => res.redirect('/admin/index.html'));

app.use(express.static('public'));

process.on('unhandledRejection', (reason) => {
    console.error('Unhandled Rejection:', reason);
});

// ==========================================
// 6. Start — seed DB first, then listen
// ==========================================
seedIfNeeded(db, () => {
    app.listen(port, () => console.log(`🚀 Server running on port ${port}`));
});
