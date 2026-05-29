# ui_components.py
import streamlit as st

class UIComponents:
    @staticmethod
    def apply_custom_styles():
        """Apply custom CSS styles (from original)"""
        st.markdown("""
        <style>
            .main-title {
                font-size: 2.8rem;
                background: linear-gradient(90deg, #FF6B6B, #4ECDC4);
                -webkit-background-clip: text;
                -webkit-text-fill-color: transparent;
                text-align: center;
                margin-bottom: 1rem;
                font-weight: bold;
            }
            
            .subtitle {
                font-size: 1.2rem;
                color: #666;
                text-align: center;
                margin-bottom: 2rem;
            }
            
            .upload-box {
                border: 2px dashed #4ECDC4;
                border-radius: 10px;
                padding: 30px;
                text-align: center;
                background-color: #f8f9fa;
                margin-bottom: 20px;
            }
            
            .lyrics-box {
                border: 1px solid #ddd;
                border-radius: 8px;
                padding: 15px;
                height: 400px;
                overflow-y: auto;
                background-color: #fff;
                box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            }
            
            .playback-controls {
                display: flex;
                justify-content: center;
                gap: 10px;
                margin: 20px 0;
                padding: 15px;
                background-color: #f0f2f6;
                border-radius: 10px;
            }
            
            .status-box {
                padding: 10px;
                border-radius: 5px;
                margin: 10px 0;
                font-weight: bold;
            }
            
            .success {
                background-color: #d4edda;
                color: #155724;
                border: 1px solid #c3e6cb;
            }
            
            .warning {
                background-color: #fff3cd;
                color: #856404;
                border: 1px solid #ffeaa7;
            }
            
            .info {
                background-color: #d1ecf1;
                color: #0c5460;
                border: 1px solid #bee5eb;
            }
            
            .player-container {
                background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
                border-radius: 15px;
                padding: 20px;
                color: white;
                margin: 20px 0;
            }
        </style>
        """, unsafe_allow_html=True)
