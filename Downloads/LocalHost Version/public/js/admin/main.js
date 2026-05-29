/* * IERG 4210 - Admin Panel Logic (Complete Phase 4 Version)
 * Path: public/js/admin/main.js
 */
(function() {
    const token = sessionStorage.getItem('csrfToken');
    console.log("Checking Frontend Token:", token); // <--- 檢查這行
    if (!token) {
        window.location.href = '../login.html';
    }
})();

document.addEventListener('DOMContentLoaded', async () => {
    console.log("Admin System Initialized");

    // 1. 初始化：載入分類下拉選單
    const catSelect = document.getElementById('cat-select');
    if (catSelect) {
        try {
            const categories = await API.getCategories();
            if (categories && categories.length > 0) {
                catSelect.innerHTML = categories.map(c => 
                    `<option value="${c.catid}">${escapeHTML(c.name)}</option>`
                ).join('');
            }
        } catch (err) {
            console.error("Failed to load categories:", err);
        }
    }

    // 2. 處理產品上傳邏輯
    const productForm = document.getElementById('product-form');
    if (productForm) {
        productForm.onsubmit = async (e) => {
            e.preventDefault();
            
            // 建立 FormData 物件 (包含圖片檔案)
            const formData = new FormData(productForm);

            try {
                // 使用 API.secureFetch 確保 Request Header 帶有 x-csrf-token
                const res = await API.secureFetch('/api/admin/product', {
                    method: 'POST',
                    body: formData
                    // 注意：這裡不可手動設定 Content-Type，FormData 會自動處理
                });

                if (res && res.ok) {
                    alert('✅ 產品建立成功！');
                    productForm.reset();
                    // (選填) 這裡可以加上刷新產品列表的邏輯
                } else {
                    const errorData = await res.json();
                    alert(`❌ 上傳失敗: ${errorData.message || '請檢查 CSRF/Auth 狀態'}`);
                }
            } catch (err) {
                console.error("Upload process error:", err);
                alert("伺服器連線錯誤，請稍後再試。");
            }
        };
    }
});

/**
 * 安全助手：轉義 HTML 防止 XSS
 */
function escapeHTML(str) {
    if (!str) return "";
    return str.replace(/[&<>"']/g, m => ({
        '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;'
    }[m]));
}