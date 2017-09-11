using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.IO;

namespace VSADriver
{
    class Program
    {
        //[DllImport("PInvokeCPP.dll", EntryPoint = "print", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        //static extern void print(string msg, StringBuilder dest, int n);

        [DllImport("../../../Debug/VSA.dll", EntryPoint = "generate_plans", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void generate_plans(string input, StringBuilder plans, int len);

        static void Main(string[] args)
        {
            string input = File.ReadAllText("../../SampleInputOutput/input.json");
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
