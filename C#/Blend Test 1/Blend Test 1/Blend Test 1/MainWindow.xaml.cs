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

namespace Blend_Test_1
{
    //public class TickThread
    //{
    //    public void Tick()
    //    {
    //        while (true)
    //        {
    //            if (m_bMouseOver)
    //            {
    //                System.Diagnostics.Debug.WriteLine("Tick");

    //                this.m_LabelToEdit.Begin

                    
    //                m_LabelToEdit.Content = "Hello World";
    //            }


    //            System.Threading.Thread.Sleep(1000);
    //        }
    //    }

    //    bool m_bMouseOver = false;
    //    Label m_LabelToEdit;

    //    public void SetMouseOver(bool bMouseOver)
    //    {
    //        m_bMouseOver = bMouseOver;
    //    }

    //    public void SetLabelToEdit(Label pLabel)
    //    {
    //        m_LabelToEdit = pLabel;
    //    }


    //}
    
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
    }
}
