/**
 * Phase 5: 管理員訂單顯示邏輯 (優化版)
 */
document.addEventListener('DOMContentLoaded', async () => {
    const container = document.getElementById('orders-container');
    if (!container) return;

    try {
        // 呼叫後端 API 獲取所有訂單
        const response = await fetch('/api/admin/orders', {
            headers: { 
                'x-csrf-token': sessionStorage.getItem('csrfToken') 
            }
        });

        if (response.status === 401 || response.status === 403) {
            container.innerHTML = `<p style="color:red; padding: 20px;">存取被拒：請重新以管理員身份登入。</p>`;
            return;
        }

        if (!response.ok) throw new Error("Failed to fetch orders");

        const orders = await response.json();

        if (orders.length === 0) {
            container.innerHTML = "<p style='padding: 20px; color: #666;'>No orders found in database.</p>";
            return;
        }

        // 渲染訂單列表
        container.innerHTML = orders.map(order => {
            let items = [];
            try {
                items = JSON.parse(order.items_json); 
            } catch (e) {
                console.error("Parse error for Order #" + order.oid);
            }

            // 根據支付狀態決定顏色
            const isPaid = order.status === 'PAID';
            const statusBg = isPaid ? '#d4edda' : '#fff3cd';
            const statusColor = isPaid ? '#155724' : '#856404';
            const statusBorder = isPaid ? '#c3e6cb' : '#ffeeba';

            return `
                <div class="order-card" style="background: white; border: 1px solid #ddd; padding: 18px; margin-bottom: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.05);">
                    <div style="display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid #eee; padding-bottom: 10px; margin-bottom: 12px;">
                        <strong style="font-size: 1.1em; color: #333;">Order ID: #${order.oid}</strong>
                        <span style="padding: 4px 12px; border-radius: 20px; font-size: 0.85em; font-weight: bold; background: ${statusBg}; color: ${statusColor}; border: 1px solid ${statusBorder};">
                            ${order.status}
                        </span>
                    </div>
                    
                    <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 8px; font-size: 0.9em; color: #555; margin-bottom: 15px;">
                        <div><strong>Buyer ID:</strong> ${order.userid}</div>
                        <div><strong>Currency:</strong> ${order.currency.toUpperCase()}</div>
                        <div><strong>Date:</strong> ${new Date(order.created_at).toLocaleString()}</div>
                        <div><strong>Total:</strong> <span style="color: #d9534f; font-weight: bold;">$${order.total_price.toFixed(1)}</span></div>
                    </div>

                    <div style="font-size: 0.9em; color: #444; background: #f9f9f9; padding: 12px; border-radius: 6px; border-left: 4px solid #ddd;">
                        <strong style="display: block; margin-bottom: 6px; color: #333;">Items Summary:</strong>
                        <ul style="margin: 0; padding-left: 20px; list-style-type: circle;">
                            ${items.map(i => `
                                <li style="margin-bottom: 4px;">
                                    <span style="color: #740084; font-weight: 500;">${i.name}</span> 
                                    <span style="color: #888;">(x${i.quantity})</span> 
                                    — $${i.unit_price}
                                </li>
                            `).join('')}
                        </ul>
                    </div>
                </div>
            `;
        }).join('');

    } catch (err) {
        container.innerHTML = `<div style="padding: 20px; background: #fff5f5; color: #c53030; border-radius: 8px; border: 1px solid #feb2b2;">
            <strong>Error:</strong> ${err.message}
        </div>`;
    }
});