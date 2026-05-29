using System.Windows;
using System.Windows.Controls;

namespace miniproject
{
    public partial class EndGamePage : Page
    {
        public EndGamePage()
        {
            InitializeComponent();
        }

        private void BtnReturn_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new SongSelectionPage());
        }
    }
}
