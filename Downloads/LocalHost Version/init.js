/* Phase 4 - Database Reset & Complete Initialization (Final Version) */
const sqlite3 = require('sqlite3').verbose();
const crypto = require('crypto');

const dbPath = path.resolve(__dirname, 'database', 'shop.db');
const db = new sqlite3.Database(dbPath);

// 加密助手函數 (PBKDF2)
function hashPassword(password) {
    const salt = crypto.randomBytes(16).toString('hex');
    const hash = crypto.pbkdf2Sync(password, salt, 1000, 64, 'sha512').toString('hex');
    return { salt, hash };
}

db.serialize(() => {
    console.log("正在根據 Phase 4 標準重置資料庫...");

    // 1. 清除舊表
    db.run("DROP TABLE IF EXISTS products");
    db.run("DROP TABLE IF EXISTS categories");
    db.run("DROP TABLE IF EXISTS users");

    // 2. 建立 Categories 表
    db.run(`CREATE TABLE categories (
        catid INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT UNIQUE NOT NULL
    )`);

    // 3. 建立 Products 表
    db.run(`CREATE TABLE products (
        pid INTEGER PRIMARY KEY AUTOINCREMENT,
        catid INTEGER,
        name TEXT UNIQUE NOT NULL,
        price REAL NOT NULL,
        description TEXT,
        image TEXT, 
        FOREIGN KEY (catid) REFERENCES categories(catid)
    )`);

    // 4. 建立 Users 表 (依據評分表要求：userid, email, password，並增加 isAdmin) [cite: 39, 41]
    db.run(`CREATE TABLE users (
        userid INTEGER PRIMARY KEY AUTOINCREMENT,
        email TEXT UNIQUE NOT NULL,
        password TEXT NOT NULL,
        salt TEXT NOT NULL,
        isAdmin INTEGER DEFAULT 0 
    )`);

    // Phase 5 - Orders Table
    db.run(`CREATE TABLE IF NOT EXISTS orders (
        oid INTEGER PRIMARY KEY AUTOINCREMENT,
        userid INTEGER,
        total_price REAL NOT NULL,
        currency TEXT NOT NULL,
        items_json TEXT NOT NULL,  -- 存儲 [{pid, qty, unit_price}]
        salt TEXT NOT NULL,        -- 隨機鹽值 [cite: 25]
        digest TEXT NOT NULL,      -- H(order, salt) [cite: 22]
        status TEXT DEFAULT 'PENDING',
        stripe_session_id TEXT,    -- 用於回傳時校驗
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (userid) REFERENCES users(userid)
    )`);

    // 建立 discounts 表
    db.run(`CREATE TABLE IF NOT EXISTS discounts (
        did INTEGER PRIMARY KEY AUTOINCREMENT,
        pid INTEGER UNIQUE,            -- 每個產品暫定一個優惠規則
        type TEXT NOT NULL,           -- 'B2G1' (買2送1) 或 'BULK' (多買多平)
        threshold INTEGER NOT NULL,    -- 門檻數量 (例如 3 或 2)
        discount_val REAL,             -- 如果是 BULK，這就是一組的總價
        FOREIGN KEY (pid) REFERENCES products(pid)
    )`);

    /* ===== 插入初始資料 ===== */
    
    // 插入分類
    const catStmt = db.prepare("INSERT INTO categories (name) VALUES (?)");
    ["Full Professors", "Assistant Professors", "Lecturers"].forEach(name => catStmt.run(name));
    catStmt.finalize();

    // 插入範例產品
    const prodStmt = db.prepare("INSERT INTO products (catid, name, price, description, image) VALUES (?, ?, ?, ?, ?)");
    // Category 1: Full Professors (CHAN CHUN KIT)
    prodStmt.run(1, "Prof. CHAN CHUN KIT - Thesis Supervision", 12345.0, "Advanced consultation for graduate research and final year projects.", "chanck.jpg");

    // Category 2: Assistant Professors (YAN WEIJING)
    prodStmt.run(2, "Prof. YAN WEIJING - Assistant Professor Lab", 1.0, "Specialized research guidance and academic mentoring sessions.", "yanwj.jpg");

    // Category 3: Lecturers (MA PUI KIT & YIN HO FAI)
    prodStmt.run(3, "Mr. MA PUI KIT (Jack Ma) - Phase 6 Expert Clinic", 1000000.0, "Deep dive into web extension implementation and security headers.", "jackma.jpg");
    prodStmt.run(3, "Mr. YIN HO FAI (Hoover) - Peer Hacking Defense", 67676767.0, "Preparing your Azure VM for the Phase 7 security challenges.", "hoover.jpg");

    prodStmt.finalize();

    const discStmt = db.prepare("INSERT INTO discounts (pid, type, threshold, discount_val) VALUES (?, ?, ?, ?)");
    discStmt.run(1, 'B2G1', 3, null);
    discStmt.run(3, 'BULK', 2, 1000.0);

    discStmt.finalize();


    // 5. 插入帳號 [cite: 41, 42]
    // 管理員帳號 (isAdmin = 1)
    const adminData = hashPassword('admin123');
    db.run("INSERT INTO users (email, password, salt, isAdmin) VALUES (?, ?, ?, ?)", 
        ['admin@example.com', adminData.hash, adminData.salt, 1]);

    // 普通用戶帳號 (isAdmin = 0)
    const userData = hashPassword('user123');
    db.run("INSERT INTO users (email, password, salt, isAdmin) VALUES (?, ?, ?, ?)", 
        ['user@example.com', userData.hash, userData.salt, 0], (err) => {
        if (err) {
            console.error("❌ 初始化帳號失敗:", err.message);
        } else {
            console.log("-----------------------------------------");
            console.log("✅ 資料庫已成功重置！");
            console.log("👤 [管理員] admin@example.com / admin123");
            console.log("👤 [普通用戶] user@example.com / user123");
            console.log("-----------------------------------------");
        }
    });
});

db.close();