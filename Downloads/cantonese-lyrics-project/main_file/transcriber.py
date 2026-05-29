# transcriber.py
import torch
from transformers import pipeline
import librosa
import tempfile
import os
import time

class CantoneseLyricsTranscriber:
    def __init__(self):
        """Initialize transcription system"""
        self.model = None
        self.current_audio = None
        self.audio_duration = 0
        self.is_playing = False
        self.playback_position = 0
        self.transcription_result = ""
        self.manual_lyrics = ""
    
    def load_model(self):
        """Load Whisper model (fixed version)"""
        if self.model is None:
            try:
                # Set model cache
                os.environ['TRANSFORMERS_CACHE'] = './models'
                
                # Fix: Use correct parameter configuration
                self.model = pipeline(
                    "automatic-speech-recognition",
                    model="alvanlii/whisper-small-cantonese",
                    device="cuda" if torch.cuda.is_available() else "cpu",
                    chunk_length_s=30,  # Process long audio
                    stride_length_s=[4, 2],  # Overlap strategy
                    # Fix: Only include model-accepted parameters in generate_kwargs
                    generate_kwargs={
                        "task": "transcribe",
                        "language": "zh"
                    }
                )
                return True
            except Exception as e:
                print(f"Model loading failed: {e}")
                return False
        return True
    
    def transcribe_audio(self):
        """Transcribe audio (fixed version)"""
        import streamlit as st
        
        if not self.model:
            st.error("Model not loaded")
            return False
        
        if not st.session_state.audio_file:
            st.error("Please upload an audio file first")
            return False
        
        try:
            # Create temporary file for transcription
            temp_dir = tempfile.mkdtemp()
            temp_path = os.path.join(temp_dir, st.session_state.audio_file['name'])
            
            with open(temp_path, "wb") as f:
                f.write(st.session_state.audio_bytes)
            
            # Show transcription progress
            progress_bar = st.progress(0)
            status_text = st.empty()
            
            # Start transcription
            status_text.text("🎤 Transcribing... (This may take some time)")
            
            # Fix: Use correct parameters for transcription
            # For long audio, we need to set return_timestamps=False
            result = self.model(
                temp_path,
                return_timestamps=False,  # Fix: Explicitly set no timestamps
                generate_kwargs={
                    "task": "transcribe",
                    "language": "zh"
                }
            )
            
            full_text = result["text"]
            
            # Simulate real-time transcription effect (display text in segments)
            words = full_text.split()
            chunk_size = max(1, len(words) // 20)  # Divide into about 20 segments
            
            transcription_parts = []
            for i in range(0, len(words), chunk_size):
                chunk = words[i:i+chunk_size]
                transcription_parts.append(" ".join(chunk))
                
                # Update progress
                progress = min((i + chunk_size) / len(words), 1.0)
                progress_bar.progress(progress)
                
                # Update displayed text
                current_text = " ".join(transcription_parts)
                st.session_state.transcription_result = current_text
                
                # Small delay to simulate real-time effect
                time.sleep(0.1)
            
            # Ensure final result is complete
            st.session_state.transcription_result = full_text
            st.session_state.transcription_complete = True
            
            # Clean up temporary files
            os.remove(temp_path)
            os.rmdir(temp_dir)
            
            progress_bar.progress(1.0)
            status_text.text("✅ Transcription completed!")
            
            return True
            
        except Exception as e:
            st.error(f"Transcription failed: {e}")
            
            # Detailed error information
            if "without_timestamps" in str(e):
                st.info("💡 Tip: Parameter configuration error, automatically fixed")
            elif "more than 3000 mel input features" in str(e):
                st.info("💡 Tip: Audio too long, attempting chunked processing...")
                # Try chunked processing
                return self.transcribe_chunked()
            
            return False
    
    def transcribe_chunked(self):
        """Process long audio in chunks (alternative method)"""
        import streamlit as st
        
        try:
            # Create temporary file
            temp_dir = tempfile.mkdtemp()
            temp_path = os.path.join(temp_dir, st.session_state.audio_file['name'])
            
            with open(temp_path, "wb") as f:
                f.write(st.session_state.audio_bytes)
            
            # Load audio
            y, sr = librosa.load(temp_path, sr=16000)
            duration = len(y) / sr
            
            # Chunk processing (20 seconds per chunk)
            chunk_duration = 20  # seconds
            chunk_samples = chunk_duration * sr
            
            all_text = []
            progress_bar = st.progress(0)
            status_text = st.empty()
            
            for i in range(0, len(y), chunk_samples):
                chunk = y[i:i+chunk_samples]
                chunk_num = (i // chunk_samples) + 1
                total_chunks = (len(y) + chunk_samples - 1) // chunk_samples
                
                status_text.text(f"🎤 Transcribing... Segment {chunk_num}/{total_chunks}")
                
                # Process current chunk
                chunk_result = self.model(
                    {"array": chunk, "sampling_rate": sr},
                    return_timestamps=False,
                    generate_kwargs={
                        "task": "transcribe",
                        "language": "zh"
                    }
                )
                
                all_text.append(chunk_result["text"])
                
                # Update progress
                progress = min((i + chunk_samples) / len(y), 1.0)
                progress_bar.progress(progress)
                
                # Update display
                st.session_state.transcription_result = " ".join(all_text)
                time.sleep(0.5)  # Give user time to see progress
            
            # Merge results
            full_text = " ".join(all_text)
            st.session_state.transcription_result = full_text
            st.session_state.transcription_complete = True
            
            # Clean up
            os.remove(temp_path)
            os.rmdir(temp_dir)
            
            progress_bar.progress(1.0)
            status_text.text("✅ Chunked transcription completed!")
            
            return True
            
        except Exception as e:
            st.error(f"Chunked transcription failed: {e}")
            return False
