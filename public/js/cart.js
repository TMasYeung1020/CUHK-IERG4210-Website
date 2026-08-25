const Cart = {
    items: JSON.parse(localStorage.getItem('cart')) || {}, 
    
    save() { 
        localStorage.setItem('cart', JSON.stringify(this.items)); 
        this.render(); 
    },

    addItem(pid, name, price) {
        this.items[pid] = this.items[pid] || { name, price: parseFloat(price), qty: 0 };
        this.items[pid].qty++;
        this.save();
    },

    updateQty(pid, qty) {
        const newQty = parseInt(qty);
        if (newQty <= 0) {
            delete this.items[pid];
        } else {
            this.items[pid].qty = newQty;
        }
        this.save();
    },

    calculateDiscount(qty, price, rule) {
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
    },

    async render() {
        const list = document.getElementById('cart-items-list');
        const totalSpan = document.getElementById('cart-total');
        let total = 0;
        if (!list) return;
        
        let htmlContent = '';

        for (let pid in this.items) {
            const item = this.items[pid];
            
            // 修正點：使用 API.js 封裝好的函式，並處理錯誤
            let discountRule = null;
            try {
                if (typeof API !== 'undefined' && API.getDiscount) {
                    discountRule = await API.getDiscount(pid);
                }
            } catch (e) { 
                console.error("Discount fetch error for pid:", pid, e); 
            }

            const originalSubtotal = item.price * item.qty;
            const finalSubtotal = this.calculateDiscount(item.qty, item.price, discountRule);
            total += finalSubtotal;

            const hasDiscount = finalSubtotal < originalSubtotal;

            htmlContent += `
                <div class="cart-item" style="border-bottom: 1px solid #eee; padding: 10px 0; display: flex; flex-direction: column;">
                    <div style="font-weight: bold; margin-bottom: 5px;">${item.name}</div>
                    <div style="display: flex; justify-content: space-between; align-items: center;">
                        <div style="display: flex; align-items: center; gap: 5px;">
                            <span style="font-size: 0.8em; color: #666;">Qty:</span>
                            <input type="number" min="0" value="${item.qty}" 
                                   onchange="Cart.updateQty('${pid}', this.value)" 
                                   style="width: 40px; padding: 2px;">
                        </div>
                        <span style="font-size: 0.9em;">
                            ${hasDiscount ? `<del style="color: #999; margin-right: 5px;">$${originalSubtotal.toFixed(1)}</del>` : ''}
                            <span style="${hasDiscount ? 'color: #d9534f; font-weight: bold;' : ''}">
                                $${finalSubtotal.toFixed(1)}
                            </span>
                        </span>
                    </div>
                </div>`;
        }

        list.innerHTML = htmlContent || '<p style="color: #666; text-align: center; padding: 20px 0;">Your cart is empty</p>';
        if (totalSpan) totalSpan.innerText = total.toFixed(1);
    }
};

document.addEventListener('DOMContentLoaded', () => Cart.render());