# app.py
import streamlit as st
import os
import tempfile
import numpy as np
import io
from pathlib import Path
import torch
from transformers import pipeline
import librosa
import soundfile as sf
import time
import json
from datetime import datetime
import base64

from transcriber import CantoneseLyricsTranscriber
from audio_utils import AudioProcessor
from ui_components import UIComponents

# Page configuration
st.set_page_config(
    page_title="Cantonese Lyrics Transcription System",
    page_icon="🎵",
    layout="wide",
    initial_sidebar_state="expanded"
)

class CantoneseLyricsApp:
    def __init__(self):
        """Initialize transcription system"""
        self.transcriber = CantoneseLyricsTranscriber()
        self.ui = UIComponents()
        
        # Apply custom CSS
        self.ui.apply_custom_styles()
        
        # Initialize session state
        self.init_session_state()
        
    def init_session_state(self):
        """Initialize session state variables"""
        if 'transcription_result' not in st.session_state:
            st.session_state.transcription_result = ""
        if 'manual_lyrics' not in st.session_state:
            st.session_state.manual_lyrics = ""
        if 'audio_file' not in st.session_state:
            st.session_state.audio_file = None
        if 'audio_bytes' not in st.session_state:
            st.session_state.audio_bytes = None
        if 'is_transcribing' not in st.session_state:
            st.session_state.is_transcribing = False
        if 'transcription_complete' not in st.session_state:
            st.session_state.transcription_complete = False
        if 'playback_started' not in st.session_state:
            st.session_state.playback_started = False
        if 'current_time' not in st.session_state:
            st.session_state.current_time = 0
    
    def run(self):
        """Run main application"""
        # Page title
        st.markdown('<h1 class="main-title">🎵 Cantonese Lyrics Synchronous Transcription System</h1>', unsafe_allow_html=True)
        st.markdown('<p class="subtitle">Upload Cantonese song → Automatic playback → Synchronous lyrics transcription</p>', unsafe_allow_html=True)
        
        # Sidebar
        with st.sidebar:
            st.image("https://www.ie.cuhk.edu.hk/wp-content/uploads/2023/04/IE_logo-150x150.png", width=100)
            st.markdown("### Yeung Man, Thomas (1155203181)")
            st.markdown("### 🎯 IERG 4320 Project")
            st.markdown("""
            **Lyrics Detection Model:**
            - 🎤 Cantonese speech recognition
            - 🎵 Real-time audio playback
            - 📝 Two-way lyrics editing
            - ⏱️ Synchronous transcription display
            
            **How to use:**
            1. Upload WAV format song
            2. Click "Start Transcription"
            3. Song automatically plays
            4. Watch real-time transcription
            """)
            
            # System status
            st.markdown("---")
            st.markdown("### 📊 System Status")
            if torch.cuda.is_available():
                st.success("✅ GPU available")
            else:
                st.warning("⚠️ Using CPU (slower)")
            
            if self.transcriber.model:
                st.success("✅ Model loaded")
            else:
                st.info("ℹ️ Model preparing...")
        
        # Main content area
        tab1, tab2, = st.tabs(["🎵 Upload & Transcribe", "📊 Result Analysis"])

        with tab1:
            st.markdown("### Upload Song File")
            st.markdown('<div class="upload-box">', unsafe_allow_html=True)

            uploaded_file = st.file_uploader(
                "Select WAV format audio file",
                type=['wav'],
                help="Please upload Cantonese song WAV file",
                label_visibility="collapsed",
                key="file_uploader"
            )

            st.markdown('</div>', unsafe_allow_html=True)

            if uploaded_file:
                # Process uploaded file
                success, message = AudioProcessor.process_uploaded_file(uploaded_file)
                if success:
                    st.success(f"Uploaded: {uploaded_file.name}")
                    
                    # Create audio player
                    if st.session_state.audio_bytes:
                        audio_html = AudioProcessor.create_audio_player_html(st.session_state.audio_bytes)
                        st.components.v1.html(audio_html, height=150)
                    
                    # Load model if needed
                    if self.transcriber.load_model():
                        st.markdown("---")

                        # Display lyrics editors
                        self.display_lyrics_editors()

        with tab2:
            st.markdown("### Transcription Result Analysis")

            # Both conditions must be met: 1. File uploaded  2. Transcription completed
            if not st.session_state.get('audio_file') or not st.session_state.get('transcription_complete'):
                st.info("Please upload a song and complete transcription in the 'Upload & Transcribe' page")
                st.stop()

            # At this point, there must be file + completed transcription
            self.display_control_panel()  # Duration + transcription status + character count

            st.markdown("---")

            text = st.session_state.transcription_result
            col1, col2, col3, col4 = st.columns(4)
            col1.metric("Total Characters", len(text))
            col2.metric("Chinese Characters", sum('\u4e00' <= c <= '\u9fff' for c in text))
            col3.metric("Lines", text.count('\n') + 1)
            col4.metric("Punctuation Count", sum(c in '，。！？；：「」『』、' for c in text))

            st.markdown("---")
            st.markdown("#### Download Lyrics File")

            col_dl1, col_dl2 = st.columns(2)

            # Left: Download AI transcription result (always available)
            with col_dl2:
                ai_name = f"{st.session_state.audio_file['name'].rsplit('.',1)[0]}-AI-transcription.txt"
                st.download_button(
                    label="Download AI Transcribed Lyrics",
                    data=st.session_state.transcription_result,
                    file_name=ai_name,
                    mime="text/plain",
                    type="primary",
                    use_container_width=True
                )

            # Right: Download manual lyrics (only show if input exists)
            with col_dl1:
                manual_name = f"{st.session_state.audio_file['name'].rsplit('.',1)[0]}-manual-correction.txt"
                st.download_button(
                    label="Download Manual Corrected Lyrics",
                    data=st.session_state.manual_lyrics,
                    file_name=manual_name,
                    mime="text/plain",
                    type="secondary",
                    use_container_width=True
                )
    
    def display_lyrics_editors(self):
        """Display lyrics editors (from original)"""
        col1, col2 = st.columns(2)
        
        with col1:
            st.markdown("### ✏️ Manual Lyrics Input")
            st.caption("Manually input or edit lyrics here")
            
            # Manual input box
            manual_lyrics = st.text_area(
                "Manual Lyrics",
                value=st.session_state.manual_lyrics,
                height=350,
                placeholder="Enter lyrics here...\n\nExample:\nDream together, let me cast away worries\nWeave words of love together\nRight or wrong, many joys and sorrows part and meet",
                key="manual_lyrics_input"
            )
            
            if manual_lyrics != st.session_state.manual_lyrics:
                st.session_state.manual_lyrics = manual_lyrics
        
        with col2:
            st.markdown("### 🤖 Automatic Transcription Lyrics")
            st.caption("AI automatically recognized lyrics result")
            
            # Display transcription result
            transcription_display = st.text_area(
                "Transcription Result",
                value=st.session_state.transcription_result,
                height=350,
                placeholder="Transcription result will display here...",
                key="transcription_display"
            )
        
        # Add spacing
        st.markdown("<br>", unsafe_allow_html=True)
        
        # Transcription control button - FULL WIDTH
        if st.button("🎤 Start Transcription", 
                   type="primary", 
                   use_container_width=True,
                   disabled=st.session_state.is_transcribing or not st.session_state.audio_file,
                   key="start_transcribe"):
            st.session_state.is_transcribing = True
            with st.spinner("Transcribing..."):
                success = self.transcriber.transcribe_audio()
                st.session_state.is_transcribing = False
                if success:
                    st.rerun()
    
    def display_control_panel(self):
        """Display control panel (from original)"""
        st.markdown("### 🎛️ Control Panel")
        
        col1, col2, col3 = st.columns(3)
        
        with col1:
            st.metric("Audio Duration", f"{st.session_state.audio_file['duration']:.1f}s" 
                     if st.session_state.audio_file else "N/A")
        
        with col2:
            status_icon = "🟢" if st.session_state.transcription_complete else \
                         "🟡" if st.session_state.is_transcribing else "⚪"
            status_text = "Completed" if st.session_state.transcription_complete else \
                         "In Progress" if st.session_state.is_transcribing else "Waiting"
            st.metric("Transcription Status", f"{status_icon} {status_text}")
        
        with col3:
            char_count = len(st.session_state.transcription_result) if st.session_state.transcription_result else 0
            st.metric("Lyrics Character Count", char_count)

def main():
    """Main function"""
    # Initialize application
    app = CantoneseLyricsApp()
    
    # Run application
    app.run()

if __name__ == "__main__":
    main()
