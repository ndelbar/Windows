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
using System.Threading;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace Blend_Test_1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            m_threadTick = new Thread(Tick);

            m_threadTick.Start();
        }

        public void Tick()
        {
            while (true)
            {
                TimeSpan tSpan = TimeSpan.FromSeconds(m_nTotalTime);

                if (m_bMouseOver)
                {
                    this.Dispatcher.Invoke(() =>
                    {
                        m_nTotalTime++;
                        TimeDuration.Content = tSpan.ToString("hh':'mm':'ss");
                    });
                    Thread.Sleep(999);
                }
                Thread.Sleep(1);
            }
        }

        private void eventMouseEnter(object sender, MouseEventArgs e)
        {
            m_bMouseOver = true;
        }

        private void eventMouseLeave(object sender, MouseEventArgs e)
        {
            m_bMouseOver = false;
        }

        private void appClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            m_threadTick.Abort();
            m_threadTick.Join();
        }

        Thread m_threadTick;
        bool m_bMouseOver = false;
        int m_nTotalTime = 0;

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;

        [DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd,
                         int Msg, int wParam, int lParam);
        [DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();

        private void Form1_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ButtonState == MouseButtonState.Pressed)
            {
                MoveWindowNormally();
            }
        }

        private void MoveWindowNormally()
        {
            ReleaseCapture();
            Window window = Window.GetWindow(this);
            var wih = new WindowInteropHelper(window);
            IntPtr hWnd = wih.Handle;
            SendMessage(hWnd, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
        }
    }
}
