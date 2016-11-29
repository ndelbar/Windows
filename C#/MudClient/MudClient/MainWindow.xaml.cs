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
using System.Text.RegularExpressions;



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
                byte[] data = new byte[10000];

                while (m_bRunListeningThread)
                {
                    if (m_bConnected)
                    {
                        NetworkStream ns = m_Server.GetStream();

                        System.Threading.Thread.Sleep(100);

                        if (m_strAppend.Length == 0 && ns.DataAvailable)
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
                            AppendTextToMainWindow(m_strAppend);
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

        public void AppendTextToMainWindow(string strMessage)
        {
            // This strategy gets rid of text colors, and keeps the code simple. 
            // Adding color to text will be a future project
            // Example of the color info is:
            // There are five obvious exits: [0m[1m[32meast[0m, [0m[1m[32mnorth[0m, [0m[1m[32msouth[0m, [0m[1m[32mup[0m and [0m[1m[32mwest[0m

            strMessage = strMessage.Replace("\r\n", "\r");
            strMessage = Regex.Replace(strMessage, @"[\[\]].*?m", string.Empty);
            m_strMainWindow.AppendText(strMessage);

            m_strMainWindow.ScrollToEnd();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                m_Server = new TcpClient("MidnightSun2.org", 3000);
                m_Server.NoDelay = true;
                m_Server.Client.NoDelay = true;
            }
            catch (SocketException)
            {
                m_strMainWindow.AppendText("Unable to connect...");
            }

            m_bConnected = true;
        }

        bool m_bRunListeningThread = true;
        bool m_bConnected = false;
        String m_strAppend = "";
        TcpClient m_Server;

        private void SendCommandToServer()
        {
            NetworkStream ns = m_Server.GetStream();

            while (true)
            {
                if (ns.CanWrite)
                {
                    try
                    {
                        byte[] myWriteBuffer = Encoding.ASCII.GetBytes(m_strInput.Text + "\r\n");
                        ns.Write(myWriteBuffer, 0, myWriteBuffer.Length);
                        m_strInput.Text = "";
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("{0} First exception caught.", ex);
                    }

                    break;
                }
            }
        }

        private void Button_Click_Send(object sender, RoutedEventArgs e)
        {
            SendCommandToServer();
        }

        private void Button_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                SendCommandToServer();
        }
    }

}
