using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UIDesign
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window//继承Windows类
    {
        public MainWindow()
        {
            InitializeComponent();//加载各种组件

        }

        private void Call_Click(object sender, RoutedEventArgs e)
        {
            if(text3.Text=="")
            {
                text3.Text = "Calling";
            }
            else text3.Text = "";
        }

        private void Call1_Click(object sender, RoutedEventArgs e)
        {
            if (text1.Text == "")
            {
                text1.Text = "Calling";
            }
            else text1.Text = "";
        }

        private void Send_Click(object sender, RoutedEventArgs e)
        {
            string content = textBox2.Text;


        }
    }
}
