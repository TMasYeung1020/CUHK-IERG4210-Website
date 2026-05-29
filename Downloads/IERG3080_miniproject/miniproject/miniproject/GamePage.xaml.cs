using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace miniproject
{
    public enum ResultState
    {
        Sad,
        Normal,
        Happy
    }

    public partial class GamePage : Page
    {
        private const double BasePixelSpeed = 350.0;
        private double SpawnX = 800;
        private const double HitX = 79;

        private TranslateTransform _upperBgScroll;
        private Canvas _playfield;
        private TextBlock _scoreText;
        private TextBlock _judgeText;
        private Image _imgRimLeft, _imgRimRight, _imgCenterLeft, _imgCenterRight;

        private MediaPlayer _mediaPlayer;
        private MediaPlayer _donSoundPlayer = new MediaPlayer();
        private MediaPlayer _kaSoundPlayer = new MediaPlayer();

        private List<ActiveNote> _activeVisuals = new List<ActiveNote>(100);
        private List<NoteEvent> _allSongNotes = new List<NoteEvent>();
        private ImageSource _noteFaceSmall;
        private ImageSource _noteFaceBig;

        private int _nextNoteIndex = 0;
        private bool _isPlaying = false;

        private int _score = 0;
        private int _combo = 0;
        private int _maxCombo = 0;
        private int _perfectCount = 0;
        private int _goodCount = 0;
        private int _badCount = 0;

        private Uri DrumAssetUri => new Uri("pack://application:,,,/Assets/drum.png", UriKind.Absolute);
        private Uri NoteAssetUri => new Uri("pack://application:,,,/Assets/note.png", UriKind.Absolute);
        private Uri LevelAssetUri => new Uri("pack://application:,,,/Assets/level.png", UriKind.Absolute);

        public GamePage(string courseName = "Oni")
        {
            InitializeComponent();
            LoadHitSounds();

            this.SizeChanged += (s, e) =>
            {
                if (Playfield != null)
                {
                    SpawnX = Playfield.ActualWidth - 50;
                }
            };

            _playfield = FindName("Playfield") as Canvas;
            _scoreText = FindName("ScoreText") as TextBlock;
            _upperBgScroll = FindName("UpperBgTransform") as TranslateTransform;
            _judgeText = FindName("JudgeText") as TextBlock;
            _imgRimLeft = FindName("ImgRimLeft") as Image;
            _imgRimRight = FindName("ImgRimRight") as Image;
            _imgCenterLeft = FindName("ImgCenterLeft") as Image;
            _imgCenterRight = FindName("ImgCenterRight") as Image;

            _mediaPlayer = new MediaPlayer();
            _mediaPlayer.MediaFailed += (s, e) => MessageBox.Show($"Audio Failed: {e.ErrorException}");
            _mediaPlayer.MediaEnded += OnSongEnded;

            LoadDrumAssets();
            LoadNoteAssets();
            RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.HighQuality);
            CompositionTarget.Rendering += OnGameLoop;

            if (courseName != null)
            {
                string tjaPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Assets", "test.tja");
                if (File.Exists(tjaPath))
                {
                    LoadAndPlayTja(tjaPath, courseName);
                }
                else
                {
                    MessageBox.Show("test.tja not found in Assets folder!");
                }
            }

            this.Focusable = true;
            this.Loaded += (s, e) => this.Focus();
        }

        private void OnSongEnded(object s, EventArgs e)
        {
            _isPlaying = false;
            var state = GetResultState();
            Dispatcher.Invoke(() =>
            {
                NavigationService?.Navigate(new happyResult(
                    state,
                    _score,
                    _maxCombo,
                    _perfectCount,
                    _goodCount,
                    _badCount
                ));
            });
        }

        private void LoadHitSounds()
        {
            string donPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Assets", "se_don.mp3");
            string kaPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Assets", "se_ka.mp3");

            if (File.Exists(donPath)) _donSoundPlayer.Open(new Uri(donPath, UriKind.Absolute));
            if (File.Exists(kaPath)) _kaSoundPlayer.Open(new Uri(kaPath, UriKind.Absolute));
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            this.Focus();
        }

        private void LoadDrumAssets()
        {
            try
            {
                var bmp = new BitmapImage();
                bmp.BeginInit();
                bmp.UriSource = DrumAssetUri;
                bmp.CacheOption = BitmapCacheOption.OnLoad;
                bmp.EndInit();
                bmp.Freeze();

                int w = bmp.PixelWidth;
                int h = bmp.PixelHeight / 5;

                ImageSource Crop(int i) => new CroppedBitmap(bmp, new Int32Rect(0, i * h, w, h));

                var drumBase = FindName("ImgDrumBase") as Image;
                if (drumBase != null) drumBase.Source = Crop(0);
                _imgRimLeft.Source = Crop(1);
                _imgRimRight.Source = Crop(2);
                _imgCenterLeft.Source = Crop(3);
                _imgCenterRight.Source = Crop(4);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load drum.png: " + ex.Message);
            }
        }

        private void LoadNoteAssets()
        {
            try
            {
                var bmp = new BitmapImage();
                bmp.BeginInit();
                bmp.UriSource = NoteAssetUri;
                bmp.CacheOption = BitmapCacheOption.OnLoad;
                bmp.EndInit();
                bmp.Freeze();

                int w = bmp.PixelWidth;
                int h = bmp.PixelHeight / 4;

                _noteFaceSmall = new CroppedBitmap(bmp, new Int32Rect(0, 0 * h, w, h));
                _noteFaceSmall.Freeze();

                _noteFaceBig = new CroppedBitmap(bmp, new Int32Rect(0, 3 * h, w, h));
                _noteFaceBig.Freeze();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load note.png: " + ex.Message);
            }
        }

        private void SetLevelIcon(string courseName)
        {
            try
            {
                var bmp = new BitmapImage();
                bmp.BeginInit();
                bmp.UriSource = LevelAssetUri;
                bmp.CacheOption = BitmapCacheOption.OnLoad;
                bmp.EndInit();
                bmp.Freeze();

                int index = 3; // default Oni
                switch (courseName.ToLower())
                {
                    case "easy": index = 0; break;
                    case "normal": index = 1; break;
                    case "hard": index = 2; break;
                    case "oni": index = 3; break;
                    case "edit": index = 4; break;
                }

                int w = bmp.PixelWidth;
                int h = bmp.PixelHeight / 5;

                var cropped = new CroppedBitmap(bmp, new Int32Rect(0, index * h, w, h));
                var levelIcon = FindName("ImgLevelIcon") as Image;
                if (levelIcon != null) levelIcon.Source = cropped;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to load level.png: " + ex.Message);
            }
        }

        public void LoadAndPlayTja(string path, string courseName)
        {
            _mediaPlayer.Stop();
            _activeVisuals.Clear();
            _allSongNotes.Clear();
            _nextNoteIndex = 0;

            _score = 0;
            _combo = 0;
            _maxCombo = 0;
            _perfectCount = 0;
            _goodCount = 0;
            _badCount = 0;

            UpdateHUD();
            _isPlaying = false;

            var toRemove = new List<UIElement>();
            foreach (UIElement c in _playfield.Children)
                if (c is Grid g && g.Tag?.ToString() == "NOTE") toRemove.Add(c);
            foreach (var item in toRemove) _playfield.Children.Remove(item);

            SetLevelIcon(courseName);

            string content = File.ReadAllText(path, Encoding.UTF8);
            var parser = new TjaParser();
            var doc = parser.Parse(content, courseName);
            if (doc == null || doc.Events.Count == 0) return;

            foreach (var ev in doc.Events) ev.Time -= doc.OffsetSeconds;
            _allSongNotes = doc.Events;

            _mediaPlayer.MediaEnded -= OnSongEnded;
            _mediaPlayer = new MediaPlayer();
            _mediaPlayer.MediaEnded += OnSongEnded;

            string folder = System.IO.Path.GetDirectoryName(path);
            string audioPath = System.IO.Path.Combine(folder, doc.WaveFile);
            if (!File.Exists(audioPath))
            {
                string mp3Path = System.IO.Path.Combine(folder, System.IO.Path.GetFileNameWithoutExtension(doc.WaveFile) + ".mp3");
                if (File.Exists(mp3Path)) audioPath = mp3Path;
            }

            if (File.Exists(audioPath))
            {
                _mediaPlayer.Open(new Uri(audioPath, UriKind.Absolute));
                _mediaPlayer.Play();
                _isPlaying = true;
            }
        }

        private void OnGameLoop(object sender, EventArgs e)
        {
            if (_upperBgScroll != null)
            {
                _upperBgScroll.X -= 1.0;
                double imageWidth = this.ActualWidth;
                if (_upperBgScroll.X <= -imageWidth) _upperBgScroll.X = 0;
            }

            if (!_isPlaying || _mediaPlayer.Source == null) return;

            double currentAudioTime = _mediaPlayer.Position.TotalSeconds;
            double badWindow = 0.150;
            double userSpeedMultiplier = SetupPage.ScrollSpeed;

            // Spawn notes
            while (_nextNoteIndex < _allSongNotes.Count)
            {
                var note = _allSongNotes[_nextNoteIndex];
                double speed = BasePixelSpeed * note.ScrollSpeed * userSpeedMultiplier;
                double distance = SpawnX - HitX;
                double timeToTravel = distance / speed;
                double spawnTime = note.Time - timeToTravel;

                if (currentAudioTime >= spawnTime)
                {
                    SpawnNoteVisual(note, speed, distance);
                    _nextNoteIndex++;
                }
                else break;
            }

            // Update and check notes
            for (int i = _activeVisuals.Count - 1; i >= 0; i--)
            {
                var active = _activeVisuals[i];
                double speed = BasePixelSpeed * active.EventData.ScrollSpeed * userSpeedMultiplier;
                double timeDifference = active.EventData.Time - currentAudioTime;
                double newX = HitX + (timeDifference * speed);
                Canvas.SetLeft(active.VisualShape, newX);

                // Miss detection (only for head)
                if (!active.HeadProcessed && timeDifference < -badWindow)
                {
                    active.HeadProcessed = true;
                    if (active.EventData.Type <= 6)
                    {
                        _combo = 0;
                        _badCount++;
                        ShowJudgment("BAD", Brushes.Gray);
                        UpdateHUD();
                    }
                }

                // Remove only when the entire note (including long tail) is off-screen
                double noteWidth = active.VisualShape.ActualWidth;
                double rightEdgeX = newX + noteWidth;

                if (rightEdgeX < -50)
                {
                    RemoveVisual(i);
                }
            }
        }

        private void RemoveVisual(int index)
        {
            if (index < 0 || index >= _activeVisuals.Count) return;
            _playfield.Children.Remove(_activeVisuals[index].VisualShape);
            _activeVisuals.RemoveAt(index);
        }

        private void SpawnNoteVisual(NoteEvent ev, double speed, double dist)
        {
            bool isBig = (ev.Type == 3 || ev.Type == 4 || ev.Type == 6);
            double size = isBig ? 58 : 38;

            Brush fill = Brushes.Gray;
            if (ev.Type == 1 || ev.Type == 3) fill = new SolidColorBrush(Color.FromRgb(240, 50, 50));
            else if (ev.Type == 2 || ev.Type == 4) fill = new SolidColorBrush(Color.FromRgb(50, 150, 255));
            else if (ev.Type == 5 || ev.Type == 6) fill = Brushes.Gold;
            else if (ev.Type == 7) fill = Brushes.Orange;

            ImageSource faceToUse = isBig ? _noteFaceBig : _noteFaceSmall;

            Grid container = new Grid { Height = size, Tag = "NOTE" };

            bool isLong = (ev.Type == 5 || ev.Type == 6) && ev.Duration > 0;

            if (isLong)
            {
                double longWidth = speed * ev.Duration;
                if (longWidth < size) longWidth = size;
                container.Width = longWidth;

                var rect = new Rectangle
                {
                    Width = longWidth,
                    Height = size,
                    RadiusX = size / 2,
                    RadiusY = size / 2,
                    Fill = fill,
                    Stroke = Brushes.Black,
                    StrokeThickness = 2
                };
                container.Children.Add(rect);

                if (faceToUse != null)
                {
                    var face = new Image
                    {
                        Source = faceToUse,
                        Width = size,
                        Height = size,
                        Stretch = Stretch.Uniform,
                        HorizontalAlignment = HorizontalAlignment.Left
                    };
                    container.Children.Add(face);
                }
            }
            else
            {
                container.Width = size;

                var ellipse = new Ellipse
                {
                    Width = size,
                    Height = size,
                    Fill = fill,
                    Stroke = Brushes.Black,
                    StrokeThickness = 2
                };
                container.Children.Add(ellipse);

                if (faceToUse != null)
                {
                    var face = new Image
                    {
                        Source = faceToUse,
                        Width = size,
                        Height = size,
                        Stretch = Stretch.Uniform
                    };
                    container.Children.Add(face);
                }
            }

            Panel.SetZIndex(container, 10);
            Canvas.SetTop(container, 90 - (size / 2));
            Canvas.SetLeft(container, SpawnX);
            _playfield.Children.Add(container);

            _activeVisuals.Add(new ActiveNote { EventData = ev, VisualShape = container });
        }

        private void Page_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.IsRepeat) return;
            Key key = e.Key == Key.System ? e.SystemKey : e.Key;

            bool isHit = (key == SetupPage.P1_Key1 || key == SetupPage.P1_Key2 ||
                          key == SetupPage.P1_Key3 || key == SetupPage.P1_Key4 ||
                          key == SetupPage.P2_Key1 || key == SetupPage.P2_Key2 ||
                          key == SetupPage.P2_Key3 || key == SetupPage.P2_Key4);
            if (!isHit) return;

            if (key == SetupPage.P1_Key1 || key == SetupPage.P2_Key1) _imgRimLeft.Opacity = 1;
            if (key == SetupPage.P1_Key2 || key == SetupPage.P2_Key2) _imgCenterLeft.Opacity = 1;
            if (key == SetupPage.P1_Key3 || key == SetupPage.P2_Key3) _imgCenterRight.Opacity = 1;
            if (key == SetupPage.P1_Key4 || key == SetupPage.P2_Key4) _imgRimRight.Opacity = 1;

            bool isDon = (key == SetupPage.P1_Key2 || key == SetupPage.P1_Key3 ||
                          key == SetupPage.P2_Key2 || key == SetupPage.P2_Key3);

            if (isDon)
            {
                _donSoundPlayer.Position = TimeSpan.Zero;
                _donSoundPlayer.Play();
            }
            else
            {
                _kaSoundPlayer.Position = TimeSpan.Zero;
                _kaSoundPlayer.Play();
            }

            if (_isPlaying)
                CheckHitOptimized(isDon ? 1 : 2);
        }

        private void Page_KeyUp(object sender, KeyEventArgs e)
        {
            Key key = e.Key == Key.System ? e.SystemKey : e.Key;
            if (key == SetupPage.P1_Key1 || key == SetupPage.P2_Key1) _imgRimLeft.Opacity = 0;
            if (key == SetupPage.P1_Key2 || key == SetupPage.P2_Key2) _imgCenterLeft.Opacity = 0;
            if (key == SetupPage.P1_Key3 || key == SetupPage.P2_Key3) _imgCenterRight.Opacity = 0;
            if (key == SetupPage.P1_Key4 || key == SetupPage.P2_Key4) _imgRimRight.Opacity = 0;
        }

        private void CheckHitOptimized(int inputType)
        {
            double currentTime = _mediaPlayer.Position.TotalSeconds;
            double goodWindow = 0.050;
            double okWindow = 0.150;

            ActiveNote bestNote = null;
            double minDiff = double.MaxValue;

            foreach (var n in _activeVisuals)
            {
                if (!n.HeadProcessed && n.EventData.Type >= 1 && n.EventData.Type <= 6)
                {
                    double diff = Math.Abs(n.EventData.Time - currentTime);
                    if (diff <= okWindow && diff < minDiff)
                    {
                        minDiff = diff;
                        bestNote = n;
                    }
                }
            }

            if (bestNote != null)
            {
                bool isRed = (bestNote.EventData.Type == 1 || bestNote.EventData.Type == 3);
                bool isBlue = (bestNote.EventData.Type == 2 || bestNote.EventData.Type == 4);
                bool isRoll = (bestNote.EventData.Type == 5 || bestNote.EventData.Type == 6);
                bool match = isRoll || (inputType == 1 && isRed) || (inputType == 2 && isBlue);

                if (match)
                {
                    bestNote.IsHit = true;
                    bestNote.HeadProcessed = true;

                    // Hide normal notes immediately after hit, but NOT rolls
                    if (!isRoll)
                    {
                        bestNote.VisualShape.Visibility = Visibility.Hidden;
                    }

                    if (minDiff <= goodWindow)
                    {
                        _score += isRoll ? 0 : 100;
                        ShowJudgment("PERFECT", Brushes.Gold);
                        _perfectCount++;
                    }
                    else
                    {
                        _score += isRoll ? 0 : 50;
                        ShowJudgment("GOOD", Brushes.White);
                        _goodCount++;
                    }

                    _combo++;
                    _maxCombo = Math.Max(_maxCombo, _combo);
                    UpdateHUD();
                    return;
                }
                else if (!isRoll)
                {
                    bestNote.HeadProcessed = true;
                    _combo = 0;
                    _badCount++;
                    ShowJudgment("BAD", Brushes.BlueViolet);
                    UpdateHUD();
                    return;
                }
            }

            // Continuous roll hits
            bool insideRoll = false;
            foreach (var n in _activeVisuals)
            {
                if (n.EventData.Type == 5 || n.EventData.Type == 6)
                {
                    double endTime = n.EventData.Time + n.EventData.Duration;
                    if (currentTime >= n.EventData.Time && currentTime <= endTime)
                    {
                        insideRoll = true;
                        break;
                    }
                }
            }

            if (insideRoll)
            {
                _score += 100;
                _combo++;
                _maxCombo = Math.Max(_maxCombo, _combo);
                ShowJudgment("ROLL!", Brushes.Yellow);
                UpdateHUD();
            }
        }

        private ResultState GetResultState()
        {
            if (_score >= 8000) return ResultState.Happy;
            if (_score >= 4000) return ResultState.Normal;
            return ResultState.Sad;
        }

        private void ShowJudgment(string text, Brush color)
        {
            _judgeText.Text = text;
            _judgeText.Foreground = color;
            Canvas.SetTop(_judgeText, 30);
            var fade = new DoubleAnimation(1, 0, TimeSpan.FromSeconds(0.4));
            var move = new DoubleAnimation(30, 10, TimeSpan.FromSeconds(0.4));
            _judgeText.BeginAnimation(TextBlock.OpacityProperty, fade);
            _judgeText.BeginAnimation(Canvas.TopProperty, move);
        }

        private void UpdateHUD()
        {
            if (_scoreText != null) _scoreText.Text = _score.ToString();
        }

        private class ActiveNote
        {
            public NoteEvent EventData;
            public FrameworkElement VisualShape;
            public bool IsHit;
            public bool HeadProcessed;
        }

        private class NoteEvent
        {
            public double Time;
            public int Type;
            public double ScrollSpeed = 1.0;
            public double Duration;
        }

        private class TjaDocument
        {
            public double Bpm = 120;
            public string WaveFile;
            public double OffsetSeconds;
            public List<NoteEvent> Events = new List<NoteEvent>();
        }

        private class TjaParser
        {
            public TjaDocument Parse(string content, string targetCourse)
            {
                var doc = new TjaDocument();
                using (StringReader reader = new StringReader(content))
                {
                    string line;
                    bool inCourse = false, processing = false;
                    double curBpm = 120, curScroll = 1.0, curTime = 0;
                    var raw = new List<NoteEvent>();

                    while ((line = reader.ReadLine()) != null)
                    {
                        int cIdx = line.IndexOf("//");
                        if (cIdx >= 0) line = line.Substring(0, cIdx);
                        line = line.Trim();
                        if (string.IsNullOrEmpty(line)) continue;

                        if (!inCourse)
                        {
                            if (line.StartsWith("BPM:")) double.TryParse(line.Substring(4), NumberStyles.Any, CultureInfo.InvariantCulture, out curBpm);
                            if (line.StartsWith("WAVE:")) doc.WaveFile = line.Substring(5).Trim();
                            if (line.StartsWith("OFFSET:")) double.TryParse(line.Substring(7), NumberStyles.Any, CultureInfo.InvariantCulture, out doc.OffsetSeconds);
                            if (line.StartsWith("COURSE:") && string.Equals(line.Substring(7).Trim(), targetCourse, StringComparison.OrdinalIgnoreCase))
                            {
                                inCourse = true;
                                doc.Bpm = curBpm;
                            }
                        }
                        else
                        {
                            if (line.StartsWith("COURSE:") || line.StartsWith("#END")) break;
                            if (line.StartsWith("#START"))
                            {
                                processing = true;
                                curTime = 0;
                                continue;
                            }
                            if (!processing) continue;
                            if (line.StartsWith("#SCROLL"))
                            {
                                var p = line.Split(' ');
                                if (p.Length > 1) double.TryParse(p[1], NumberStyles.Any, CultureInfo.InvariantCulture, out curScroll);
                                continue;
                            }
                            if (line.StartsWith("BPM:"))
                            {
                                double.TryParse(line.Substring(4), NumberStyles.Any, CultureInfo.InvariantCulture, out curBpm);
                                continue;
                            }
                            if (line.EndsWith(","))
                            {
                                string data = line.TrimEnd(',');
                                if (data.Length == 0) continue;
                                double measureDuration = (60.0 / curBpm) * 4.0;
                                double charTime = measureDuration / data.Length;
                                foreach (char c in data)
                                {
                                    int t = c - '0';
                                    if (t >= 1 && t <= 9)
                                        raw.Add(new NoteEvent { Time = curTime, Type = t, ScrollSpeed = curScroll });
                                    curTime += charTime;
                                }
                            }
                        }
                    }

                    raw.Sort((a, b) => a.Time.CompareTo(b.Time));

                    for (int i = 0; i < raw.Count; i++)
                    {
                        var ev = raw[i];
                        if (ev.Type == 8) continue;
                        if (ev.Type >= 5 && ev.Type <= 7)
                        {
                            for (int j = i + 1; j < raw.Count; j++)
                            {
                                if (raw[j].Type == 8)
                                {
                                    ev.Duration = raw[j].Time - ev.Time;
                                    raw[j].Type = 0;
                                    break;
                                }
                            }
                        }
                        if (ev.Type != 0) doc.Events.Add(ev);
                    }
                }

                return doc;
            }
        }
    }
}