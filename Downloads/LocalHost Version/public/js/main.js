/* Phase 4 - Part 4.5: Secure & Dynamic Content Loading + Auth Logic */

document.addEventListener('DOMContentLoaded', async () => {
    // 檢查登入狀態並渲染導航欄
    renderAuthStatus();

    // 獲取並渲染左側分類導航
    try {
        const categories = await API.getCategories();
        const navUl = document.getElementById('category-list'); 
        
        if (navUl && categories) {
            // 渲染成 .category-item 結構
            navUl.innerHTML = categories.map((cat, index) => `
                <li class="category-item ${index === 0 ? 'active' : ''}" 
                    data-catid="${cat.catid}"
                    onclick="handleCategoryClick(this, ${cat.catid}, '${cat.name.replace(/'/g, "\\'")}')">
                    ${escapeHTML(cat.name)}
                </li>
            `).join('');

            // 預設載入第一個分類的產品
            if (categories.length > 0) {
                loadProducts(categories[0].catid, categories[0].name);
            }
        }
    } catch (err) {
        console.error("Failed to load categories:", err);
    }

    // 初始化折扣公告欄 (Phase 6 核心)
    initPromoBanner();
});

/**
 * 處理分類點擊切換樣式
 */
function handleCategoryClick(element, catid, catName) {
    document.querySelectorAll('.category-item').forEach(item => {
        item.classList.remove('active');
    });
    element.classList.add('active');
    loadProducts(catid, catName);
}

/**
 * 渲染登入狀態
 */
function renderAuthStatus() {
    const authStatus = document.getElementById('auth-status');
    const token = sessionStorage.getItem('csrfToken');
    const userEmail = sessionStorage.getItem('userEmail');
    const isAdmin = sessionStorage.getItem('isAdmin'); 

    if (!authStatus) return;

    if (token) {
        const isAdminUser = parseInt(isAdmin);
        let navLink = (isAdminUser === 1) 
            ? `<a href="admin/index.html" style="margin-right:15px; color: #740084; font-weight: bold; text-decoration: none;">[ Admin Panel ]</a>`
            : `<a href="my-orders.html" style="margin-right:15px; color: #740084; font-weight: bold; text-decoration: none;">[ My Orders ]</a>`;

        authStatus.innerHTML = `
            ${navLink}
            <span style="margin-right:10px; font-size: 0.9em; color: #666;">HI ${escapeHTML(userEmail)}</span>
            <a href="#" id="logout-link" style="color: #d9534f; margin-left: 10px;">Logout</a>
        `;
        
        document.getElementById('logout-link').onclick = async (e) => {
            e.preventDefault();
            await handleLogout();
        };
    } else {
        authStatus.innerHTML = `<a href="login.html">Login</a>`;
    }
}

/**
 * 登出處理
 */
async function handleLogout() {
    try {
        await fetch('/api/logout', { method: 'POST' });
    } catch (err) {
        console.error("Logout API failed", err);
    } finally {
        sessionStorage.clear();
        window.location.href = 'index.html';
    }
}

/**
 * 載入產品列表
 */
async function loadProducts(catid, catName) {
    const products = await API.getProducts(catid);
    const grid = document.getElementById('product-list'); 
    const breadcrumbCat = document.getElementById('breadcrumb-category');
    const catTitle = document.getElementById('cat-title');
    
    if (!grid) return;
    if (breadcrumbCat) breadcrumbCat.innerText = catName;
    if (catTitle) catTitle.innerText = catName;

    if (!products || products.length === 0) {
        grid.innerHTML = '<p style="padding: 20px;">No tutor sessions available in this category.</p>';
        return;
    }

    grid.innerHTML = products.map(p => `
        <div class="product-card">
            <a href="product.html?pid=${p.pid}" style="text-decoration: none; color: inherit;">
                <div style="aspect-ratio: 1/1; overflow: hidden; background: #f0f0f0; border-radius: 8px;">
                    <img src="/uploads/${p.image || 'placeholder.jpg'}" alt="${escapeHTML(p.name)}" style="width:100%; height:100%; object-fit: cover;">
                </div>
                <h3>${escapeHTML(p.name)}</h3>
            </a>
            <p style="font-weight: bold; color: #740084; font-size: 1.1em;">$${parseFloat(p.price).toFixed(1)}</p>
            <button class="add-to-cart-btn" onclick="Cart.addItem(${p.pid}, '${p.name.replace(/'/g, "\\'")}', ${p.price})">
                Add to Cart
            </button>
        </div>
    `).join('');
}

/**
 * 初始化優惠公告欄 (Phase 6 需求：提及折扣)
 */
async function initPromoBanner() {
    const banner = document.getElementById('promo-banner');
    const details = document.getElementById('promo-details');
    if (!banner || !details) return;

    try {
        const categories = await API.getCategories();
        let hasAnyPromo = false;
        details.innerHTML = "";

        for (let cat of categories) {
            const products = await API.getProducts(cat.catid);
            for (let p of products) {
                const disc = await API.getDiscount(p.pid);
                if (disc) {
                    hasAnyPromo = true;
                    const li = document.createElement('li');
                    li.innerHTML = disc.type === 'B2G1' 
                        ? `<strong>${p.name}</strong>: Buy 2 Get 1 FREE!` 
                        : `<strong>${p.name}</strong>: Special Price $${disc.discount_val} for every ${disc.threshold} sessions!`;
                    details.appendChild(li);
                }
            }
        }
        if (hasAnyPromo) banner.style.display = 'block';
    } catch (e) {
        console.error("Promo banner error:", e);
    }
}

function escapeHTML(str) {
    if (!str) return "";
    return str.replace(/[&<>"']/g, m => ({
        '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;'
    }[m]));
}