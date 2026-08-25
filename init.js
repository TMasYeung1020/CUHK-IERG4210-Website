const sqlite3 = require('sqlite3').verbose();
const crypto = require('crypto');
const path = require('path');

function hashPassword(password) {
    const salt = crypto.randomBytes(16).toString('hex');
    const hash = crypto.pbkdf2Sync(password, salt, 1000, 64, 'sha512').toString('hex');
    return { salt, hash };
}

function seedIfNeeded(db, callback) {
    db.serialize(() => {
        db.run(`CREATE TABLE IF NOT EXISTS categories (
            catid INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL
        )`);
        db.run(`CREATE TABLE IF NOT EXISTS products (
            pid INTEGER PRIMARY KEY AUTOINCREMENT,
            catid INTEGER,
            name TEXT UNIQUE NOT NULL,
            price REAL NOT NULL,
            description TEXT,
            image TEXT,
            FOREIGN KEY (catid) REFERENCES categories(catid)
        )`);
        db.run(`CREATE TABLE IF NOT EXISTS users (
            userid INTEGER PRIMARY KEY AUTOINCREMENT,
            email TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            salt TEXT NOT NULL,
            isAdmin INTEGER DEFAULT 0
        )`);
        db.run(`CREATE TABLE IF NOT EXISTS orders (
            oid INTEGER PRIMARY KEY AUTOINCREMENT,
            userid INTEGER,
            total_price REAL NOT NULL,
            currency TEXT NOT NULL,
            items_json TEXT NOT NULL,
            salt TEXT NOT NULL,
            digest TEXT NOT NULL,
            status TEXT DEFAULT 'PENDING',
            stripe_session_id TEXT,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (userid) REFERENCES users(userid)
        )`);
        db.run(`CREATE TABLE IF NOT EXISTS discounts (
            did INTEGER PRIMARY KEY AUTOINCREMENT,
            pid INTEGER UNIQUE,
            type TEXT NOT NULL,
            threshold INTEGER NOT NULL,
            discount_val REAL,
            FOREIGN KEY (pid) REFERENCES products(pid)
        )`);

        db.get("SELECT COUNT(*) as count FROM products", (err, row) => {
            if (err || (row && row.count > 0)) {
                if (callback) callback();
                return;
            }

            const catStmt = db.prepare("INSERT INTO categories (name) VALUES (?)");
            ["Full Professors", "Assistant Professors", "Lecturers"].forEach(name => catStmt.run(name));
            catStmt.finalize();

            const prodStmt = db.prepare("INSERT INTO products (catid, name, price, description, image) VALUES (?, ?, ?, ?, ?)");
            prodStmt.run(1, "Prof. CHAN CHUN KIT - Thesis Supervision", 12345.0, "Advanced consultation for graduate research and final year projects.", "chanck.jpg");
            prodStmt.run(2, "Prof. YAN WEIJING - Assistant Professor Lab", 1.0, "Specialized research guidance and academic mentoring sessions.", "yanwj.jpg");
            prodStmt.run(3, "Mr. MA PUI KIT (Jack Ma) - Phase 6 Expert Clinic", 1000000.0, "Deep dive into web extension implementation and security headers.", "jackma.jpg");
            prodStmt.run(3, "Mr. YIN HO FAI (Hoover) - Peer Hacking Defense", 67676767.0, "Preparing your Azure VM for the Phase 7 security challenges.", "hoover.jpg");
            prodStmt.finalize();

            const discStmt = db.prepare("INSERT INTO discounts (pid, type, threshold, discount_val) VALUES (?, ?, ?, ?)");
            discStmt.run(1, 'B2G1', 3, null);
            discStmt.run(3, 'BULK', 2, 1000.0);
            discStmt.finalize();

            const adminData = hashPassword('admin123');
            db.run("INSERT INTO users (email, password, salt, isAdmin) VALUES (?, ?, ?, ?)",
                ['admin@example.com', adminData.hash, adminData.salt, 1]);

            const userData = hashPassword('user123');
            db.run("INSERT INTO users (email, password, salt, isAdmin) VALUES (?, ?, ?, ?)",
                ['user@example.com', userData.hash, userData.salt, 0], (err) => {
                    if (err) {
                        console.error('Seed error:', err.message);
                    } else {
                        console.log('✅ DB seeded — admin@example.com/admin123 | user@example.com/user123');
                    }
                    if (callback) callback();
                });
        });
    });
}

module.exports = { seedIfNeeded };
