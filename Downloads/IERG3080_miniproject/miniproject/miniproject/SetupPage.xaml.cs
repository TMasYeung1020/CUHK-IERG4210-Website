using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace miniproject
{
    public partial class SetupPage : Page
    {
        private Button _currentKeyButton = null;

        // Static key bindings (4 per player)
        public static Key P1_Key1 { get; set; } = Key.D;
        public static Key P1_Key2 { get; set; } = Key.F;
        public static Key P1_Key3 { get; set; } = Key.J;
        public static Key P1_Key4 { get; set; } = Key.K;

        public static Key P2_Key1 { get; set; } = Key.A;
        public static Key P2_Key2 { get; set; } = Key.S;
        public static Key P2_Key3 { get; set; } = Key.L;
        public static Key P2_Key4 { get; set; } = Key.OemSemicolon; // ;

        public static int ScrollSpeed { get; set; } = 1;

        public SetupPage()
        {
            InitializeComponent();

            Btn_P1_Key1.Content = P1_Key1.ToString().ToUpper();
            Btn_P1_Key2.Content = P1_Key2.ToString().ToUpper();
            Btn_P1_Key3.Content = P1_Key3.ToString().ToUpper();
            Btn_P1_Key4.Content = P1_Key4.ToString().ToUpper();

            Btn_P2_Key1.Content = P2_Key1.ToString().ToUpper();
            Btn_P2_Key2.Content = P2_Key2.ToString().ToUpper();
            Btn_P2_Key3.Content = P2_Key3.ToString().ToUpper();
            Btn_P2_Key4.Content = P2_Key4 == Key.OemSemicolon ? ";" : P2_Key4.ToString().ToUpper();

            UpdateSpeedHighlight();
        }

        private void KeyButton_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn)
            {
                _currentKeyButton = btn;
                btn.Content = "PRESS KEY...";
                this.Focus();
            }
        }

        protected override void OnPreviewKeyDown(KeyEventArgs e)
        {
            if (_currentKeyButton != null)
            {
                e.Handled = true;
                Key key = e.Key == Key.System ? e.SystemKey : e.Key;

                if (key != Key.Escape && key != Key.Enter && key != Key.None)
                {
                    string display = key == Key.OemSemicolon ? ";" : key.ToString().ToUpper();
                    _currentKeyButton.Content = display;

                    // Save to static property
                    if (_currentKeyButton == Btn_P1_Key1) P1_Key1 = key;
                    else if (_currentKeyButton == Btn_P1_Key2) P1_Key2 = key;
                    else if (_currentKeyButton == Btn_P1_Key3) P1_Key3 = key;
                    else if (_currentKeyButton == Btn_P1_Key4) P1_Key4 = key;
                    else if (_currentKeyButton == Btn_P2_Key1) P2_Key1 = key;
                    else if (_currentKeyButton == Btn_P2_Key2) P2_Key2 = key;
                    else if (_currentKeyButton == Btn_P2_Key3) P2_Key3 = key;
                    else if (_currentKeyButton == Btn_P2_Key4) P2_Key4 = key;
                }

                _currentKeyButton = null;
            }
            base.OnPreviewKeyDown(e);
        }

        private void SpeedButton_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button btn && int.TryParse(btn.Tag?.ToString(), out int speed))
            {
                ScrollSpeed = speed;
                UpdateSpeedHighlight();
            }
        }

        private void UpdateSpeedHighlight()
        {
            Speed1x.Background = (ScrollSpeed == 1) ? Brushes.White : new SolidColorBrush(Colors.LightGray);
            Speed1x.FontWeight = (ScrollSpeed == 1) ? FontWeights.Bold : FontWeights.Normal;

            Speed2x.Background = (ScrollSpeed == 2) ? Brushes.White : new SolidColorBrush(Colors.LightGray);
            Speed2x.FontWeight = (ScrollSpeed == 2) ? FontWeights.Bold : FontWeights.Normal;

            Speed3x.Background = (ScrollSpeed == 3) ? Brushes.White : new SolidColorBrush(Colors.LightGray);
            Speed3x.FontWeight = (ScrollSpeed == 3) ? FontWeights.Bold : FontWeights.Normal;

            Speed4x.Background = (ScrollSpeed == 4) ? Brushes.White : new SolidColorBrush(Colors.LightGray);
            Speed4x.FontWeight = (ScrollSpeed == 4) ? FontWeights.Bold : FontWeights.Normal;
        }

        private void ResetButton_Click(object sender, RoutedEventArgs e)
        {
            P1_Key1 = Key.D; Btn_P1_Key1.Content = "D";
            P1_Key2 = Key.F; Btn_P1_Key2.Content = "F";
            P1_Key3 = Key.J; Btn_P1_Key3.Content = "J";
            P1_Key4 = Key.K; Btn_P1_Key4.Content = "K";

            P2_Key1 = Key.A; Btn_P2_Key1.Content = "A";
            P2_Key2 = Key.S; Btn_P2_Key2.Content = "S";
            P2_Key3 = Key.L; Btn_P2_Key3.Content = "L";
            P2_Key4 = Key.OemSemicolon; Btn_P2_Key4.Content = ";";

            ScrollSpeed = 1;
            UpdateSpeedHighlight();
        }

        private void ConfirmButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.GoBack();
        }
    }
}