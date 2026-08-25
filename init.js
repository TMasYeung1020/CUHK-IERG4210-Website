const Database = require('better-sqlite3');
const crypto = require('crypto');
const path = require('path');
const fs = require('fs');

function hashPassword(password) {
    const salt = crypto.randomBytes(16).toString('hex');
    const hash = crypto.pbkdf2Sync(password, salt, 1000, 64, 'sha512').toString('hex');
    return { salt, hash };
}

function seedIfNeeded(db) {
    db.exec(`
        CREATE TABLE IF NOT EXISTS categories (
            catid INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT UNIQUE NOT NULL
        );
        CREATE TABLE IF NOT EXISTS products (
            pid INTEGER PRIMARY KEY AUTOINCREMENT,
            catid INTEGER,
            name TEXT UNIQUE NOT NULL,
            price REAL NOT NULL,
            description TEXT,
            image TEXT,
            FOREIGN KEY (catid) REFERENCES categories(catid)
        );
        CREATE TABLE IF NOT EXISTS users (
            userid INTEGER PRIMARY KEY AUTOINCREMENT,
            email TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            salt TEXT NOT NULL,
            isAdmin INTEGER DEFAULT 0
        );
        CREATE TABLE IF NOT EXISTS orders (
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
        );
        CREATE TABLE IF NOT EXISTS discounts (
            did INTEGER PRIMARY KEY AUTOINCREMENT,
            pid INTEGER UNIQUE,
            type TEXT NOT NULL,
            threshold INTEGER NOT NULL,
            discount_val REAL,
            FOREIGN KEY (pid) REFERENCES products(pid)
        );
    `);

    const count = db.prepare('SELECT COUNT(*) as count FROM products').get();
    if (count.count > 0) return;

    db.prepare("INSERT INTO categories (name) VALUES (?)").run("Full Professors");
    db.prepare("INSERT INTO categories (name) VALUES (?)").run("Assistant Professors");
    db.prepare("INSERT INTO categories (name) VALUES (?)").run("Lecturers");

    db.prepare("INSERT INTO products (catid, name, price, description, image) VALUES (?,?,?,?,?)").run(1, "Prof. CHAN CHUN KIT - Thesis Supervision", 12345.0, "Advanced consultation for graduate research and final year projects.", "chanck.jpg");
    db.prepare("INSERT INTO products (catid, name, price, description, image) VALUES (?,?,?,?,?)").run(2, "Prof. YAN WEIJING - Assistant Professor Lab", 1.0, "Specialized research guidance and academic mentoring sessions.", "yanwj.jpg");
    db.prepare("INSERT INTO products (catid, name, price, description, image) VALUES (?,?,?,?,?)").run(3, "Mr. MA PUI KIT (Jack Ma) - Phase 6 Expert Clinic", 1000000.0, "Deep dive into web extension implementation and security headers.", "jackma.jpg");
    db.prepare("INSERT INTO products (catid, name, price, description, image) VALUES (?,?,?,?,?)").run(3, "Mr. YIN HO FAI (Hoover) - Peer Hacking Defense", 67676767.0, "Preparing your Azure VM for the Phase 7 security challenges.", "hoover.jpg");

    db.prepare("INSERT INTO discounts (pid, type, threshold, discount_val) VALUES (?,?,?,?)").run(1, 'B2G1', 3, null);
    db.prepare("INSERT INTO discounts (pid, type, threshold, discount_val) VALUES (?,?,?,?)").run(3, 'BULK', 2, 1000.0);

    const adminData = hashPassword('admin123');
    db.prepare("INSERT INTO users (email, password, salt, isAdmin) VALUES (?,?,?,?)").run('admin@example.com', adminData.hash, adminData.salt, 1);

    const userData = hashPassword('user123');
    db.prepare("INSERT INTO users (email, password, salt, isAdmin) VALUES (?,?,?,?)").run('user@example.com', userData.hash, userData.salt, 0);

    console.log('✅ DB seeded — admin@example.com/admin123 | user@example.com/user123');
}

module.exports = { seedIfNeeded };
