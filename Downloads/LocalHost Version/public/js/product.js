/* Phase 4 - Product Detail Loader */
document.addEventListener('DOMContentLoaded', async () => {
    // 1. 從 URL 獲取 pid (例如: product.html?pid=5)
    const urlParams = new URLSearchParams(window.location.search);
    const pid = urlParams.get('pid');

    if (!pid) {
        window.location.href = 'index.html';
        return;
    }

    try {
        const response = await fetch(`/api/product/${pid}`);
        const product = await response.json();

        if (!product || product.message) {
            alert("Product not found");
            window.location.href = 'index.html';
            return;
        }

    // 3. 渲染頁面內容
    document.getElementById('p-name').innerText = product.name;
    document.getElementById('p-price').innerText = `$${product.price.toFixed(1)} / hour`;
    document.getElementById('p-desc').innerText = product.description;
    document.getElementById('p-img').src = `/uploads/${product.image}`;

    // 4. 加入購物車按鈕邏輯
    const addBtn = document.getElementById('add-to-cart-btn');
    const qtyInput = document.getElementById('p-qty');

    addBtn.onclick = () => {
        const qty = parseInt(qtyInput.value) || 1;
        // 循環調用 Cart.addItem 以匹配你的 cart.js 邏輯
        for(let i=0; i < qty; i++) {
            Cart.addItem(product.pid, product.name, product.price);
        }
        alert("已加入購物清單！");
    };}
    catch (err) {
        console.error("Error fetching product:", err);
    }
});