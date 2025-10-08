using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Farand_Bootloader_App
{
    public partial class Form1 : Form
    {

        byte[] command_Bytes = new byte[64];
        byte[] rx_Data = new byte[64];
        bool Tx_Data_Ready = false;



        bool close_Serialport = false;
        public bool Close_Serialport
        {
            get
            {
                return close_Serialport;
            }
            set
            {
                close_Serialport = value;
                if (close_Serialport == true)
                {
                    timer_Tx.Stop();
                    vcp.Close_Serialport = true;
                }


            }
        }

        public Form1()
        {
            InitializeComponent();
        }


        private void Initialize_Virtual_COM_Port()
        {
            vcp.Is_Minimised = true;
            vcp.Rx_Byte_Count = 64;
            vcp.Baud_Rate = 9600;
            vcp.Communication_Response = 0x55;
            vcp.Communication_Response_Byte_Index = 2;
            vcp.Start_Communication_Byte = 0x55;
            vcp.Start_Communication_Byte_Index = 1;
            vcp.Received_Data_Ready += Vcp_Received_Data_Ready1;
            vcp.Normal_Operation_Starts += Vcp_Normal_Operation_Starts;
            vcp.Connection_Failed += Vcp_Connection_Failed;
            vcp.Connection_Ready += Vcp_Connection_Ready;

        }

        private void Vcp_Connection_Ready(object sender, EventArgs e)
        {
            timer_Tx.Start();
        }

        private void Vcp_Connection_Failed(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        private void Vcp_Normal_Operation_Starts(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        private void Vcp_Received_Data_Ready1(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Initialize_Virtual_COM_Port();
        }

        private string Convert_To_Hex_Format(int n)
        {
            string s;
            s = Convert.ToString(n, 16);
            if (s.Length < 2)
            {
                s = "0" + s;
            }
            s = "0x" + s.ToUpper();
            return s;
        }

        public void Send_Data()
        {
            //try
            //{   


            if (Tx_Data_Ready == true)
            {
                Tx_Data_Ready = false;
            }
            else
            {
                Clear_All_Buffers();
            }

            if (close_Serialport == false)
            {
                vcp.Send_Data(command_Bytes);
            }



            //}
            //catch (Exception ex)
            //{
            // Application.Exit(); 
            //MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            //}
        }

        private void Clear_All_Buffers()
        {
            for (int i = 0; i < command_Bytes.Length; i++)
            {
                command_Bytes[i] = 0;
            }
        }
    }
}
