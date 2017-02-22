using System;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
namespace PInvokeCSharp
{
    class Program
    {
       

        [DllImport("RecEngine.dll", EntryPoint = "generate_plans", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void generate_plans(string input, StringBuilder plans, int len);

        static void Main(string[] args)
        {
            string input = File.ReadAllText("input.json");
            int len = 100000;
            StringBuilder plans = new StringBuilder(len);
            plans.EnsureCapacity(len);
            generate_plans(input, plans, len);
            File.WriteAllText("output.json", plans.ToString());
            Console.WriteLine(plans.ToString());
            Console.Read();
        }
    }
}
