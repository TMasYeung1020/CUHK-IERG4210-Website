using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

namespace miniproject
{
    public partial class happyResult : Page
    {
        public happyResult()
            : this(ResultState.Normal, 0, 0, 0, 0, 0)
        {
        }

        public happyResult(ResultState state, int score, int maxCombo, int perfect, int good, int bad)
        {
            InitializeComponent();

            if (System.ComponentModel.DesignerProperties.GetIsInDesignMode(this)) return;

            // Update text（設定文字但隱藏顯示）
            var scoreText = FindName("ScoreText") as TextBlock;
            var comboText = FindName("ComboText") as TextBlock;
            var perfectText = FindName("PerfectText") as TextBlock;
            var goodText = FindName("GoodText") as TextBlock;
            var badText = FindName("BadText") as TextBlock;

            if (scoreText != null) scoreText.Text = $"Score: {score:N0}";
            if (comboText != null) comboText.Text = $"Max Combo: {maxCombo}";
            if (perfectText != null) perfectText.Text = $"Perfect: {perfect}";
            if (goodText != null) goodText.Text = $"Good: {good}";
            if (badText != null) badText.Text = $"Bad: {bad}";

            // 確保 StatsBorder 初始隱藏
            var statsBorder = FindName("StatsBorder") as Border;
            if (statsBorder != null)
            {
                statsBorder.Visibility = Visibility.Collapsed;
                statsBorder.Opacity = 0;
            }

            // Happy people visibility（不變）
            var happyPeople = FindName("happyPeople") as Image;
            if (happyPeople != null)
            {
                happyPeople.Visibility = state == ResultState.Happy ? Visibility.Visible : Visibility.Collapsed;
            }

            // Load and crop boygirl.png based on state（不變）
            try
            {
                var fullImage = new BitmapImage();
                fullImage.BeginInit();
                fullImage.UriSource = new Uri("pack://application:,,,/Assets/boygirl.png", UriKind.Absolute);
                fullImage.CacheOption = BitmapCacheOption.OnLoad;
                fullImage.EndInit();

                int w = fullImage.PixelWidth / 2;
                int h = fullImage.PixelHeight / 3;

                int rowIndex = 1; // default to Normal
                switch (state)
                {
                    case ResultState.Happy: rowIndex = 1; break;
                    case ResultState.Normal: rowIndex = 0; break;
                    case ResultState.Sad: rowIndex = 2; break;
                }

                var img3 = FindName("Image3") as Image;
                var img4 = FindName("Image4") as Image;

                if (img3 != null)
                    img3.Source = new CroppedBitmap(fullImage, new Int32Rect(0, h * rowIndex, w, h));
                if (img4 != null)
                    img4.Source = new CroppedBitmap(fullImage, new Int32Rect(w, h * rowIndex, w, h));
            }
            catch (Exception ex)
            {
                MessageBox.Show("Image load error: " + ex.Message);
            }

            // Allow click or key press to go back（不變）
            this.MouseLeftButtonDown += HappyResult_MouseOrKeyDown;
            this.KeyDown += HappyResult_MouseOrKeyDown;
        }

        private void HappyResult_MouseOrKeyDown(object sender, InputEventArgs e)
        {
            NavigationService?.Navigate(new SongSelectionPage());  // 替換為你的歌曲選擇頁
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            var open = FindResource("DoorOpenStoryboard") as Storyboard;
            if (open != null)
            {
                open.Completed += OnDoorOpenCompleted;  // 勾鉤：門開後顯示成績
                open.Begin();
            }

            // Character bounce（不變）
            var bounce = FindResource("CharacterBounceStoryboard") as Storyboard;
            if (bounce != null)
            {
                bounce.Completed += (s, args) =>
                {
                    var continuous = FindResource("ContinuousJumpStoryboard") as Storyboard;
                    continuous?.Begin();
                };
                bounce.Begin();
            }
        }

        // 新增：門開動畫結束，揭曉白色成績區塊
        private void OnDoorOpenCompleted(object sender, EventArgs e)
        {
            var statsBorder = FindName("StatsBorder") as Border;
            if (statsBorder != null)
            {
                statsBorder.Visibility = Visibility.Visible;

                // 淡入動畫
                var fadeIn = new DoubleAnimation(0, 0.8, TimeSpan.FromSeconds(0.6));
                fadeIn.EasingFunction = new QuadraticEase { EasingMode = EasingMode.EaseOut };
                statsBorder.BeginAnimation(Border.OpacityProperty, fadeIn);

                // 彈跳縮放動畫
                var scaleAnim = new DoubleAnimation(0.8, 1.1, TimeSpan.FromSeconds(0.5));
                scaleAnim.EasingFunction = new BounceEase { Bounces = 2, Bounciness = 0.6, EasingMode = EasingMode.EaseOut };
                var scaleTransform = statsBorder.RenderTransform as ScaleTransform;
                if (scaleTransform != null)
                {
                    scaleTransform.BeginAnimation(ScaleTransform.ScaleXProperty, scaleAnim);
                    scaleTransform.BeginAnimation(ScaleTransform.ScaleYProperty, scaleAnim);
                }
            }
        }
    }
}
