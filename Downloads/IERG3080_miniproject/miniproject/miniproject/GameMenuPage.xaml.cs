using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Animation;


namespace miniproject
{
    public partial class GameMenuPage : Page
    {
        public GameMenuPage()
        {
            InitializeComponent();
        }

        private void LeftTile_MouseLeftButtonDown(object sender, RoutedEventArgs e)
        {
            // go back to SongSelectionPage
            NavigationService?.Navigate(new SongSelectionPage());
        }

        private void RightTile_MouseLeftButtonDown(object sender, RoutedEventArgs e)
        {
            // go to SetupPage (settings)
            NavigationService?.Navigate(new SetupPage());
        }

        private void EasyButton_Click(object sender, RoutedEventArgs e)
        {
            // Easy ¡ú LEVEL:3 ¡ú COURSE:Easy
            NavigationService?.Navigate(new GamePage("Easy"));
        }

        private void MediumButton_Click(object sender, RoutedEventArgs e)
        {
            // Normal ¡ú LEVEL:4 ¡ú COURSE:Normal
            NavigationService?.Navigate(new GamePage("Normal"));
        }

        private void HardButton_Click(object sender, RoutedEventArgs e)
        {
            // Oni ¡ú LEVEL:7 ¡ú COURSE:Oni
            NavigationService?.Navigate(new GamePage("Oni"));
        }
    }
}
