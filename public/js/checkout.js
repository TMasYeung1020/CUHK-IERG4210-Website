/* public/js/checkout.js */

function calculateDiscount(qty, price, rule) {
    if (!rule || qty < rule.threshold) return qty * price;
    if (rule.type === 'B2G1') {
        return (qty - Math.floor(qty / rule.threshold)) * price;
    } else if (rule.type === 'BULK') {
        const sets = Math.floor(qty / rule.threshold);
        return (sets * rule.discount_val) + ((qty % rule.threshold) * price);
    }
    return qty * price;
}

async function renderCheckoutPage() {
    const container = document.getElementById('cart-list');
    const summary = document.getElementById('checkout-summary');
    const totalSpan = document.getElementById('total-price');

    const cartData = JSON.parse(localStorage.getItem('cart') || '{}');
    const pids = Object.keys(cartData);

    if (pids.length === 0) {
        container.innerHTML = '<p class="text-center" style="color:var(--text-muted);padding:40px 0;">Your shopping cart is empty.</p>';
        summary.style.display = 'none';
        return;
    }

    let total = 0;
    let html = '';

    for (const pid of pids) {
        const item = cartData[pid];
        let discountRule = null;
        try {
            discountRule = await API.getDiscount(pid);
        } catch (e) {
            console.warn(`Could not fetch discount for PID: ${pid}`);
        }

        const originalSubtotal = item.price * item.qty;
        const finalSubtotal = calculateDiscount(item.qty, item.price, discountRule);
        total += finalSubtotal;
        const hasDiscount = finalSubtotal < originalSubtotal;

        html += `
            <div class="checkout-item">
                <div>
                    <div class="checkout-item-name">${item.name}</div>
                    <div class="checkout-item-meta">Unit Price: $${item.price.toFixed(1)}</div>
                    ${hasDiscount ? '<div class="checkout-item-promo">🔥 Promotion Applied!</div>' : ''}
                </div>
                <div class="checkout-item-price">
                    <div class="qty">Qty: ${item.qty}</div>
                    <div style="margin-top:8px;">
                        ${hasDiscount ? `<del>$${originalSubtotal.toFixed(1)}</del>` : ''}
                        <span class="final${hasDiscount ? ' discounted' : ''}">$${finalSubtotal.toFixed(1)}</span>
                    </div>
                </div>
            </div>
        `;
    }

    container.innerHTML = html;
    totalSpan.innerText = total.toFixed(1);
    summary.style.display = 'block';
}

async function startCheckout() {
    const cartData = JSON.parse(localStorage.getItem('cart') || '{}');
    const items = Object.keys(cartData).map(pid => ({
        pid: parseInt(pid),
        quantity: cartData[pid].qty
    }));

    if (items.length === 0) return alert("Cart is empty");

    const btn = document.querySelector('.btn-pay');
    const originalText = btn.innerText;

    try {
        btn.disabled = true;
        btn.innerText = "Processing...";

        const response = await fetch('/api/orders/create', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ items })
        });

        const data = await response.json();

        if (data.success && data.url) {
            window.location.href = data.url;
        } else {
            alert(data.message || "Checkout failed. Please login first.");
            if (response.status === 401) window.location.href = 'login.html';
        }
    } catch (err) {
        console.error("Stripe Checkout Error:", err);
        alert("An error occurred. Please try again.");
    } finally {
        btn.disabled = false;
        btn.innerText = originalText;
    }
}
