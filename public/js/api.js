/* public/js/api.js */
const API = {
    // 公開 API
    getCategories: () => fetch('/api/categories').then(res => res.json()),
    getProducts: (catid) => fetch(`/api/products/${catid}`).then(res => res.json()),
    getProduct: (pid) => fetch(`/api/product/${pid}`).then(res => res.json()),
    
    // --- [Phase 6 新增] 獲取特定產品的折扣規則 ---
    getDiscount: (pid) => fetch(`/api/discounts/${pid}`).then(res => res.json()),

    // 管理員專用：會自動帶上 CSRF Token 的 Fetch
    secureFetch: async (url, options = {}) => {
        const token = sessionStorage.getItem('csrfToken');
        
        // 準備 Headers
        const headers = {
            ...options.headers,
            'x-csrf-token': token 
        };

        const response = await fetch(url, { ...options, headers });

        // 如果伺服器回傳 401 或 403，代表驗證失敗
        if (response.status === 401 || response.status === 403) {
            alert("Session 已過期，請重新登入。");
            sessionStorage.clear();
            window.location.href = '../login.html';
            return null;
        }
        return response;
    }
};

const auth = {
    logout: async () => {
        // 使用 POST logout 並清除 session
        await API.secureFetch('/api/logout', { method: 'POST' });
        sessionStorage.clear();
        window.location.href = '/login.html';
    }
};