namespace Farand_Bootloader_App
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.vcp = new Auto_Detect_VCP_Control.Auto_Detect_VCP_Control();
            this.timer_Tx = new System.Windows.Forms.Timer(this.components);
            this.button_Firmware_Upgrade = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // vcp
            // 
            this.vcp.Baud_Rate = 9600;
            this.vcp.Close_Serialport = false;
            this.vcp.Communication_Response = 0;
            this.vcp.Communication_Response_Byte_Index = 0;
            this.vcp.Is_Minimised = true;
            this.vcp.Location = new System.Drawing.Point(12, 55);
            this.vcp.Name = "vcp";
            this.vcp.Rx_Byte_Count = 56;
            this.vcp.Rx_Bytes = new byte[] {
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0)),
        ((byte)(0))};
            this.vcp.Size = new System.Drawing.Size(480, 160);
            this.vcp.Start_Communication_Byte = 0;
            this.vcp.Start_Communication_Byte_Index = 0;
            this.vcp.Start_VCP_Connection = false;
            this.vcp.TabIndex = 0;
            // 
            // timer_Tx
            // 
            this.timer_Tx.Interval = 125;
            this.timer_Tx.Tick += new System.EventHandler(this.timer_Tx_Tick);
            // 
            // button_Firmware_Upgrade
            // 
            this.button_Firmware_Upgrade.Location = new System.Drawing.Point(670, 405);
            this.button_Firmware_Upgrade.Name = "button_Firmware_Upgrade";
            this.button_Firmware_Upgrade.Size = new System.Drawing.Size(118, 33);
            this.button_Firmware_Upgrade.TabIndex = 1;
            this.button_Firmware_Upgrade.Text = "Upgrade Firmware";
            this.button_Firmware_Upgrade.UseVisualStyleBackColor = true;
            this.button_Firmware_Upgrade.Click += new System.EventHandler(this.button_Firmware_Upgrade_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.button_Firmware_Upgrade);
            this.Controls.Add(this.vcp);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private Auto_Detect_VCP_Control.Auto_Detect_VCP_Control vcp;
        private System.Windows.Forms.Timer timer_Tx;
        private System.Windows.Forms.Button button_Firmware_Upgrade;
    }
}

