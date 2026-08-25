# IERG 4210 — Tutor Time Booking System

[![Live Demo](https://img.shields.io/badge/Live%20Demo-cuhk--ierg4210.onrender.com-orange?style=for-the-badge)](https://cuhk-ierg4210-website.onrender.com)

**Student:** Thomas Yeung (1155203181)

---

## 🚀 Overview

Phase 5 focused on implementing a secure checkout workflow using the **Stripe API**. The system ensures transaction integrity through server-side hashing (SHA256) and protects against data tampering using Webhooks.

---

## 🛠️ Implemented Features

### 1. Frontend Checkout Integration

* **Dynamic Cart Processing:** Converts the local shopping cart (Object format) into a structured array for the Stripe API.
* **Stripe Checkout Redirect:** Users are redirected to a secure, Stripe-hosted payment page.
* **Custom Success/Cancel Pages:** After payment, users are routed back to `checkout-success.html` (with Order ID) or `checkout-cancel.html`.

### 2. Backend Order Management

* **Order Persistence:** Every checkout attempt creates a `PENDING` record in the SQLite database.
* **Digest Generation:** Implemented a SHA256 hashing mechanism to ensure order details (Price, Items, Currency) are not modified during the transition.
* **Formula:** `SHA256(Currency | MerchantEmail | Salt | ItemsJSON | TotalPrice)`



### 3. Stripe Webhook & Security

* **Asynchronous Verification:** Listens for `checkout.session.completed` events via a secure Webhook endpoint.
* **Payload Signature Check:** Uses `stripe.webhooks.constructEvent` to verify that incoming requests are authentically from Stripe.
* **Data Integrity Check:** Upon receiving a payment success signal, the server re-calculates the Digest and compares it with the database record. Only matching orders are updated to `PAID`.

---

## 📦 Technical Stack

* **Backend:** Node.js, Express.js
* **Database:** SQLite3
* **Security:** `crypto` (PBKDF2 for passwords, SHA256 for orders), `express-session`, CSRF Nonce.
* **Payment Gateway:** Stripe API & Stripe CLI

---

## 🔧 Installation & Setup

1. **Install Dependencies:**
```bash
npm install

```


2. **Environment Configuration:**
* Replace `STRIPE_SECRET_KEY` in `server.js` with your Stripe test key.
* Update `MERCHANT_EMAIL` to your registered merchant email.


3. **Run Stripe CLI (For local testing):**
```bash
stripe listen --forward-to localhost:3000/webhook

```


4. **Start the Server:**
```bash
node server.js

```



---

## 🧪 Test Accounts

| Role | Email | Password |
|------|-------|----------|
| Admin | `admin@example.com` | `admin123` |
| User | `user@example.com` | `user123` |

**Stripe Test Card:** `4242 4242 4242 4242` — any future expiry, any CVC

---

## 📂 Project Structure

* `server.js`: Contains the main logic for Order creation and Webhook verification.
* `public/js/checkout.js`: Handles frontend-to-Stripe session initiation.
* `public/checkout-success.html`: The landing page for successful transactions.
* `database/shop.db`: Stores products, categories, users, and orders.

