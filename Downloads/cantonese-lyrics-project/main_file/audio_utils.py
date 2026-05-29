# audio_utils.py
import os
import tempfile
import librosa
import base64
import streamlit as st

class AudioProcessor:
    @staticmethod
    def process_uploaded_file(uploaded_file):
        """Process uploaded audio file (from original)"""
        import streamlit as st
        
        if uploaded_file is not None:
            # Check file format
            if not uploaded_file.name.lower().endswith('.wav'):
                st.warning("⚠️ Please upload WAV format file")
                return False, "Invalid format"
            
            # Save uploaded file
            file_bytes = uploaded_file.getvalue()
            st.session_state.audio_bytes = file_bytes
            
            # Create temporary file
            temp_dir = tempfile.mkdtemp()
            temp_path = os.path.join(temp_dir, uploaded_file.name)
            
            with open(temp_path, "wb") as f:
                f.write(file_bytes)
            
            # Get audio information
            try:
                y, sr = librosa.load(temp_path, sr=None)
                duration = librosa.get_duration(y=y, sr=sr)
                
                st.session_state.audio_file = {
                    'path': temp_path,
                    'name': uploaded_file.name,
                    'duration': duration,
                    'sample_rate': sr,
                    'size': len(file_bytes)
                }
                
                # Clean up temporary files
                os.remove(temp_path)
                os.rmdir(temp_dir)
                
                return True, "Success"
                
            except Exception as e:
                st.error(f"Audio file processing failed: {e}")
                return False, str(e)
        
        return False, "No file uploaded"
    
    @staticmethod
    def create_audio_player_html(audio_bytes):
        """Create audio player HTML (from original)"""
        # Convert audio to base64
        audio_base64 = base64.b64encode(audio_bytes).decode()
        
        # Create HTML audio player
        audio_html = f"""
        <div class="player-container">
            <audio id="audioPlayer" controls autoplay style="width: 100%;">
                <source src="data:audio/wav;base64,{audio_base64}" type="audio/wav">
                Your browser does not support audio playback
            </audio>
        </div>
        <script>
            // Get audio element
            const audioPlayer = document.getElementById('audioPlayer');
            let transcriptionStarted = false;
            
            // Start transcription when playback begins
            audioPlayer.addEventListener('play', function() {{
                if (!transcriptionStarted) {{
                    transcriptionStarted = true;
                    // Add transcription start logic here
                    console.log('Playback started, beginning transcription...');
                }}
            }});
            
            // Update current time
            audioPlayer.addEventListener('timeupdate', function() {{
                window.currentTime = audioPlayer.currentTime;
            }});
        </script>
        """
        
        return audio_html
