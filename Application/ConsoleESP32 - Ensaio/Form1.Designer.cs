namespace ConsoleESP32___Ensaio
{
    partial class Form1
    {
        /// <summary>
        /// Variável de designer necessária.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpar os recursos que estão sendo usados.
        /// </summary>
        /// <param name="disposing">true se for necessário descartar os recursos gerenciados; caso contrário, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código gerado pelo Windows Form Designer

        /// <summary>
        /// Método necessário para suporte ao Designer - não modifique 
        /// o conteúdo deste método com o editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.botaoConectar = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.labelPortasDisponiveis = new System.Windows.Forms.Label();
            this.labelComandos = new System.Windows.Forms.Label();
            this.comboBoxPortas = new System.Windows.Forms.ComboBox();
            this.checkBoxCSV = new System.Windows.Forms.CheckBox();
            this.textComandos = new System.Windows.Forms.TextBox();
            this.textDadosRecebidos = new System.Windows.Forms.TextBox();
            this.labelDados = new System.Windows.Forms.Label();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.graficoTensao = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.botaoStart = new System.Windows.Forms.Button();
            this.botaoStop = new System.Windows.Forms.Button();
            this.botaoDesconectar = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.graficoTensao)).BeginInit();
            this.SuspendLayout();
            // 
            // botaoConectar
            // 
            this.botaoConectar.Location = new System.Drawing.Point(22, 79);
            this.botaoConectar.Name = "botaoConectar";
            this.botaoConectar.Size = new System.Drawing.Size(75, 23);
            this.botaoConectar.TabIndex = 0;
            this.botaoConectar.Text = "Conectar";
            this.botaoConectar.UseVisualStyleBackColor = true;
            this.botaoConectar.Click += new System.EventHandler(this.botaoConectar_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(22, 371);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 1;
            this.button2.Text = "Enviar";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // labelPortasDisponiveis
            // 
            this.labelPortasDisponiveis.AutoSize = true;
            this.labelPortasDisponiveis.Location = new System.Drawing.Point(22, 32);
            this.labelPortasDisponiveis.Name = "labelPortasDisponiveis";
            this.labelPortasDisponiveis.Size = new System.Drawing.Size(94, 13);
            this.labelPortasDisponiveis.TabIndex = 2;
            this.labelPortasDisponiveis.Text = "Portas Disponiveis";
            // 
            // labelComandos
            // 
            this.labelComandos.AutoSize = true;
            this.labelComandos.Location = new System.Drawing.Point(22, 328);
            this.labelComandos.Name = "labelComandos";
            this.labelComandos.Size = new System.Drawing.Size(57, 13);
            this.labelComandos.TabIndex = 3;
            this.labelComandos.Text = "Comandos";
            // 
            // comboBoxPortas
            // 
            this.comboBoxPortas.FormattingEnabled = true;
            this.comboBoxPortas.Location = new System.Drawing.Point(22, 52);
            this.comboBoxPortas.Name = "comboBoxPortas";
            this.comboBoxPortas.Size = new System.Drawing.Size(268, 21);
            this.comboBoxPortas.TabIndex = 4;
            // 
            // checkBoxCSV
            // 
            this.checkBoxCSV.AutoSize = true;
            this.checkBoxCSV.Location = new System.Drawing.Point(22, 416);
            this.checkBoxCSV.Name = "checkBoxCSV";
            this.checkBoxCSV.Size = new System.Drawing.Size(82, 17);
            this.checkBoxCSV.TabIndex = 5;
            this.checkBoxCSV.Text = "Gravar CSV";
            this.checkBoxCSV.UseVisualStyleBackColor = true;
            // 
            // textComandos
            // 
            this.textComandos.Location = new System.Drawing.Point(22, 345);
            this.textComandos.Name = "textComandos";
            this.textComandos.Size = new System.Drawing.Size(268, 20);
            this.textComandos.TabIndex = 6;
            // 
            // textDadosRecebidos
            // 
            this.textDadosRecebidos.Location = new System.Drawing.Point(22, 146);
            this.textDadosRecebidos.Multiline = true;
            this.textDadosRecebidos.Name = "textDadosRecebidos";
            this.textDadosRecebidos.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textDadosRecebidos.Size = new System.Drawing.Size(268, 167);
            this.textDadosRecebidos.TabIndex = 7;
            this.textDadosRecebidos.TextChanged += new System.EventHandler(this.textDadosRecebidos_TextChanged);
            // 
            // labelDados
            // 
            this.labelDados.AutoSize = true;
            this.labelDados.Location = new System.Drawing.Point(22, 127);
            this.labelDados.Name = "labelDados";
            this.labelDados.Size = new System.Drawing.Size(92, 13);
            this.labelDados.TabIndex = 8;
            this.labelDados.Text = "Dados Recebidos";
            // 
            // graficoTensao
            // 
            chartArea3.Name = "ChartArea1";
            this.graficoTensao.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this.graficoTensao.Legends.Add(legend3);
            this.graficoTensao.Location = new System.Drawing.Point(311, 31);
            this.graficoTensao.Name = "graficoTensao";
            series3.ChartArea = "ChartArea1";
            series3.Legend = "Legend1";
            series3.Name = "Series1";
            this.graficoTensao.Series.Add(series3);
            this.graficoTensao.Size = new System.Drawing.Size(524, 428);
            this.graficoTensao.TabIndex = 9;
            this.graficoTensao.Text = "chart1";
            // 
            // botaoStart
            // 
            this.botaoStart.Location = new System.Drawing.Point(104, 370);
            this.botaoStart.Name = "botaoStart";
            this.botaoStart.Size = new System.Drawing.Size(75, 23);
            this.botaoStart.TabIndex = 10;
            this.botaoStart.Text = "Start";
            this.botaoStart.UseVisualStyleBackColor = true;
            this.botaoStart.Click += new System.EventHandler(this.botaoStart_Click);
            // 
            // botaoStop
            // 
            this.botaoStop.Location = new System.Drawing.Point(186, 370);
            this.botaoStop.Name = "botaoStop";
            this.botaoStop.Size = new System.Drawing.Size(75, 23);
            this.botaoStop.TabIndex = 11;
            this.botaoStop.Text = "Stop";
            this.botaoStop.UseVisualStyleBackColor = true;
            this.botaoStop.Click += new System.EventHandler(this.botaoStop_Click);
            // 
            // botaoDesconectar
            // 
            this.botaoDesconectar.Location = new System.Drawing.Point(103, 79);
            this.botaoDesconectar.Name = "botaoDesconectar";
            this.botaoDesconectar.Size = new System.Drawing.Size(83, 23);
            this.botaoDesconectar.TabIndex = 12;
            this.botaoDesconectar.Text = "Desconectar";
            this.botaoDesconectar.UseVisualStyleBackColor = true;
            this.botaoDesconectar.Click += new System.EventHandler(this.botaoDesconectar_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(847, 471);
            this.Controls.Add(this.botaoDesconectar);
            this.Controls.Add(this.botaoStop);
            this.Controls.Add(this.botaoStart);
            this.Controls.Add(this.graficoTensao);
            this.Controls.Add(this.labelDados);
            this.Controls.Add(this.textDadosRecebidos);
            this.Controls.Add(this.textComandos);
            this.Controls.Add(this.checkBoxCSV);
            this.Controls.Add(this.comboBoxPortas);
            this.Controls.Add(this.labelComandos);
            this.Controls.Add(this.labelPortasDisponiveis);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.botaoConectar);
            this.Name = "Form1";
            this.Text = "ConsoleESP32 - Ensaio de Tração";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.graficoTensao)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button botaoConectar;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label labelPortasDisponiveis;
        private System.Windows.Forms.Label labelComandos;
        private System.Windows.Forms.ComboBox comboBoxPortas;
        private System.Windows.Forms.CheckBox checkBoxCSV;
        private System.Windows.Forms.TextBox textComandos;
        private System.Windows.Forms.TextBox textDadosRecebidos;
        private System.Windows.Forms.Label labelDados;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.DataVisualization.Charting.Chart graficoTensao;
        private System.Windows.Forms.Button botaoStart;
        private System.Windows.Forms.Button botaoStop;
        private System.Windows.Forms.Button botaoDesconectar;
    }
}

