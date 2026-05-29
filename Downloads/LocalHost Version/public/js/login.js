/* Path: public/js/login.js */
document.addEventListener('DOMContentLoaded', () => {
    const loginForm = document.getElementById('login-form');
    
    if (loginForm) {
        loginForm.onsubmit = async (e) => {
            e.preventDefault();
            
            const email = document.getElementById('email').value;
            const password = document.getElementById('password').value;
            const msgElement = document.getElementById('login-msg');

            try {
                const res = await fetch('/api/login', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ email, password })
                });

                const data = await res.json();

                if (data.success) {
                    sessionStorage.setItem('csrfToken', data.csrfToken);
                    sessionStorage.setItem('userEmail', email);
                    sessionStorage.setItem('isAdmin', data.isAdmin); // 存入 1 (Admin) 或 0 (User)
                    
                    window.location.href = (data.isAdmin === 1) ? 'admin/index.html' : 'index.html';
                }
            } catch (err) {
                console.error("Login error:", err);
                if (msgElement) msgElement.innerText = "Server connection error";
            }
        };
    }
});