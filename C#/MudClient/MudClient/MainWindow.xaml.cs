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
using System.Net;
using System.Net.Sockets;
using System.ComponentModel;
using System.IO;



namespace MudClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

           Thread thread = new Thread(new ThreadStart(ListeningThreadFunction));
           thread.Start();
           Thread threadUI = new Thread(new ThreadStart(UpdateUIThread));
           threadUI.Start();
        }

        ~MainWindow()
        {
            m_bRunListeningThread = false;
            m_bConnected = false;
        }

        override protected void OnClosing(CancelEventArgs e)
        {
            m_bRunListeningThread = false;
            m_bConnected = false;
        }

        public void ListeningThreadFunction()
        {
            try
            {
                byte[] data = new byte[50];

                while (m_bRunListeningThread)
                {
                    if (m_bConnected)
                    {
                        NetworkStream ns = m_Server.GetStream();

                        System.Threading.Thread.Sleep(100);

                        if (m_strAppend.Length == 0)
                        {
                            int recv = ns.Read(data, 0, data.Length);
                            m_strAppend = Encoding.ASCII.GetString(data, 0, recv);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.Beep();
            }
        }

        public void UpdateUIThread()
        {
            try
            {
                while (m_bRunListeningThread)
                {
                    if (m_strAppend.Length > 0)
                    {
                        this.Dispatcher.Invoke(() =>
                        {
                            m_strMainWindow.Text += m_strAppend;
                            m_strAppend = "";
                        });
                    }
                }
            }
            catch (Exception e)
            {
                Console.Beep();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                m_Server = new TcpClient("MidnightSun2.org", 3000);
            }
            catch (SocketException)
            {
                m_strMainWindow.Text = "Unable to connect...";
            }

            m_bConnected = true;
        }

        bool m_bRunListeningThread = true;
        bool m_bConnected = false;
        String m_strAppend = "";
        TcpClient m_Server;

        private void Button_Click_Send(object sender, RoutedEventArgs e)
        {

        }
    }

}
