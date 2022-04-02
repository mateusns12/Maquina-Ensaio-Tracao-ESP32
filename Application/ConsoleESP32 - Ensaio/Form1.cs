using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Windows.Forms.DataVisualization.Charting;

namespace ConsoleESP32___Ensaio
{
    public partial class Form1 : Form
    {
        private SerialPort SerPort;
        private string DadosRecebidos;

        private string[] split = new string[2]; 
        private double _split1, _split2;
        private double[] Split1, Split2; 
        private int counter; 
        private string foutput;

        public Form1()
        {
            InitializeComponent();
            ExibirPortasDisponiveis();
            initGrafico();

            Split1 = new double[1000];
            Split2 = new double[1000];

        }

        void ExibirPortasDisponiveis()
        {
            String[] ports = SerialPort.GetPortNames();
            comboBoxPortas.Items.AddRange(ports);
        }

        private void botaoConectar_Click(object sender, EventArgs e)
        {
            SerPort = new SerialPort(); 
            
                SerPort.BaudRate = 115200;
                SerPort.PortName = comboBoxPortas.Text;
                SerPort.Parity = Parity.None;
                SerPort.DataBits = 8;
                SerPort.StopBits = StopBits.One;
                SerPort.ReadBufferSize = 200000000;
                SerPort.DataReceived += SerPort_DataReceived;
            
            try
            {
                SerPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error...!");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                SerPort.Close();
            }
            catch { }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SerPort.WriteLine(textComandos.Text.ToString());
        }

        private void SerPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            DadosRecebidos = SerPort.ReadLine(); 

            this.Invoke(new Action(ProcessingData));
        }

        private void botaoStart_Click(object sender, EventArgs e)
        {
            try
            {
                SerPort.WriteLine("t");
            }
            catch
            {
                MessageBox.Show("Porta desconectada","Erro");
            }
        }

        private void botaoStop_Click(object sender, EventArgs e)
        {
            try
            {
                SerPort.WriteLine("s");
            }
            catch
            {
                MessageBox.Show("Porta desconectada", "Erro");
            }
        }

        private void botaoDesconectar_Click(object sender, EventArgs e)
        {
            SerPort.Close();
        }

        private void textDadosRecebidos_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void ProcessingData()
        {
            try
            {                
                textDadosRecebidos.Text += DadosRecebidos.ToString() + Environment.NewLine;

                split = DadosRecebidos.Split('\t'); 

                
                Double.TryParse(split[0], out _split1);
                Double.TryParse(split[1], out _split2);
                
                Split1[counter] = _split1;
                Split2[counter] = _split2;
                
                graficoTensao.Series["Channel1"].Points.AddXY(Split1[counter], Split2[counter]);            
                graficoTensao.ChartAreas[0].RecalculateAxesScale();

                foutput = _split1.ToString() + "," + _split2.ToString() + Environment.NewLine;                

                if (checkBoxCSV.Checked == true) 
                {
                    using (StreamWriter sw = File.AppendText("Outputfile.csv"))
                    {
                        sw.Write(foutput); 
                    }

                }
            }
            catch
            { } counter++;
        }

        private void initGrafico()
        {
            graficoTensao.ChartAreas[0].AxisX.IsStartedFromZero = true; 
            graficoTensao.ChartAreas[0].AxisX.MajorGrid.LineWidth = 0; 
            graficoTensao.ChartAreas[0].AxisY.MajorGrid.LineWidth = 0;
            graficoTensao.ChartAreas[0].AxisX.Title = "Deformação (%)";
            graficoTensao.ChartAreas[0].AxisY.Title = "Tensão (MPa)";
            graficoTensao.ChartAreas[0].AxisX.Enabled = AxisEnabled.True;
            graficoTensao.ChartAreas[0].AxisY.Enabled = AxisEnabled.True;
            graficoTensao.ChartAreas[0].CursorX.AutoScroll = true;

           
            Series series = new Series()
            {
                Name = "Channel1",
                Color = System.Drawing.Color.Black,
                ChartType = SeriesChartType.FastLine,
                IsXValueIndexed = true
            };

            graficoTensao.Series.Add(series);
        }
    }
}
