/* public/js/checkout.js */

// 1. 複用與 Cart.js 相同的折扣計算邏輯，確保前後端一致
function calculateDiscount(qty, price, rule) {
    if (!rule || qty < rule.threshold) return qty * price;
    
    if (rule.type === 'B2G1') {
        const freeItems = Math.floor(qty / rule.threshold);
        return (qty - freeItems) * price;
    } else if (rule.type === 'BULK') {
        const sets = Math.floor(qty / rule.threshold);
        const remainder = qty % rule.threshold;
        return (sets * rule.discount_val) + (remainder * price);
    }
    return qty * price;
}

// 2. 渲染結帳頁面內容 (支援非同步獲取折扣)
async function renderCheckoutPage() {
    const container = document.getElementById('cart-list');
    const summary = document.getElementById('checkout-summary');
    const totalSpan = document.getElementById('total-price');

    // 從 localStorage 讀取購物車數據
    const cartData = JSON.parse(localStorage.getItem('cart') || '{}');
    const pids = Object.keys(cartData);

    if (pids.length === 0) {
        container.innerHTML = '<p style="text-align:center; color:#999; padding:40px;">Your shopping cart is empty.</p>';
        summary.style.display = 'none';
        return;
    }

    let total = 0;
    let html = '';

    for (const pid of pids) {
        const item = cartData[pid];
        
        // 透過 API 獲取該產品的折扣規則
        let discountRule = null;
        try {
            // 調用 api.js 裡的 API 物件
            discountRule = await API.getDiscount(pid);
        } catch (e) {
            console.warn(`Could not fetch discount for PID: ${pid}`);
        }

        const originalSubtotal = item.price * item.qty;
        const finalSubtotal = calculateDiscount(item.qty, item.price, discountRule);
        total += finalSubtotal;

        const hasDiscount = finalSubtotal < originalSubtotal;

        html += `
            <div class="cart-item">
                <div>
                    <strong style="font-size:1.1em; color: #333;">${item.name}</strong>
                    <div style="color:#666; font-size:0.9em;">Unit Price: $${item.price.toFixed(1)}</div>
                    ${hasDiscount ? `<div style="color:#d9534f; font-size:0.85em; font-weight:bold; margin-top:4px;">🔥 Promotion Applied!</div>` : ''}
                </div>
                <div style="text-align: right;">
                    <div style="font-weight: 500;">Quantity: ${item.qty}</div>
                    <div style="margin-top: 8px;">
                        ${hasDiscount ? `<del style="color:#999; font-size:0.85em; margin-right:8px;">$${originalSubtotal.toFixed(1)}</del>` : ''}
                        <span style="${hasDiscount ? 'color:#d9534f; font-weight:bold; font-size:1.1em;' : 'font-weight:bold;'}">
                            $${finalSubtotal.toFixed(1)}
                        </span>
                    </div>
                </div>
            </div>
        `;
    }

    container.innerHTML = html;
    totalSpan.innerText = total.toFixed(1);
    summary.style.display = 'block';
}

// 3. 處理點擊付款按鈕：呼叫後端 API 並跳轉到 Stripe
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

        // 發送到 server.js 裡的 /api/orders/create
        const response = await fetch('/api/orders/create', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ items: items })
        });

        const data = await response.json();

        if (data.success && data.url) {
            // 支付成功預備，清空購物車（或是等成功回傳後再清，建議跳轉前清空）
            // localStorage.removeItem('cart'); 
            window.location.href = data.url; // 跳轉到 Stripe Checkout 頁面
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