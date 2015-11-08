using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXMtx = Microsoft.DirectX.Matrix;

namespace MathRegression
{
    class Mtx3Regression
    {
        public void RunTests()
        {
            Matrix3 a, b;
            DXMtx dxa, dxb;

            a = new Matrix3(3, 1, 2, 1, 1, 2, 2, 3, 1);
            b = new Matrix3(4, 2, 1, 3, 1, 2, 2, 1, 2);
            dxa = new DXMtx();
            dxb = new DXMtx();

            dxa.M11 = 3; dxa.M12 = 1; dxa.M13 = 2;
            dxa.M21 = 1; dxa.M22 = 1; dxa.M23 = 2;
            dxa.M31 = 2; dxa.M32 = 3; dxa.M33 = 1;
            dxb.M11 = 4; dxb.M12 = 2; dxb.M13 = 1;
            dxb.M21 = 3; dxb.M22 = 1; dxb.M23 = 2;
            dxb.M31 = 2; dxb.M32 = 1; dxb.M33 = 2;

            Roll = 45;
            Yaw = 10;
            Pitch = 30;

            TestAdd(a, b, dxa, dxb);
            TestSub(a, b, dxa, dxb);
            TestMul(a, b, dxa, dxb);
            TestTranspose(a, dxa);
            TestInvert(a, dxa);
            TestRotations();

            PrintResults(a, b);
        }


        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }

        public void PrintResults(Matrix3 a, Matrix3 b)
        {
            Console.WriteLine("Matrix 3 Tests:");
            Console.WriteLine("A: {0}\nB: {1}", a, b);
            Console.WriteLine("A + B: {0} ({1}) -- {2}", AdditionResults[0], AdditionResults[1], Passed(AdditionResults[0] == AdditionResults[1]));
            Console.WriteLine("A - B: {0} ({1}) -- {2}", SubtractionResults[0], SubtractionResults[1], Passed(SubtractionResults[0] == SubtractionResults[1]));
            Console.WriteLine("A * B: {0} ({1}) -- {2}", MultiplyResults[0], MultiplyResults[1], Passed(MultiplyResults[0] == MultiplyResults[1]));
            Console.WriteLine("A^T: {0} ({1}) -- {2}", TransposeResults[0], TransposeResults[1], Passed(TransposeResults[0] == TransposeResults[1]));
            Console.WriteLine("A^-1: {0} ({1}) -- {2}", InverseResults[0], InverseResults[1], Passed(InverseResults[0] == InverseResults[1]));
            Console.WriteLine("Basic rotation results -- {2}\n{0}\n{1}", BasicRotations[0], BasicRotations[1], Passed(BasicRotations[0] == BasicRotations[1]));
            Console.WriteLine("Euler rotation results -- {2}\n{0}\n{1}", EulerRotations[0], EulerRotations[1], Passed(EulerRotations[0] == EulerRotations[1]));
            Console.WriteLine("AxisAngle rotation results -- {2}\n{0}\n{1}", AxisAngleRotations[0], AxisAngleRotations[1], Passed(AxisAngleRotations[0] == AxisAngleRotations[1]));
            Console.WriteLine("Quaternion rotation results -- {2}\n{0}\n{1}", QuaternionRotations[0], QuaternionRotations[1], Passed(QuaternionRotations[0] == QuaternionRotations[1]));

            Console.WriteLine("Basic == Euler: {0}", Passed(BasicRotations[0] == EulerRotations[0]));
            Console.WriteLine("Basic == AxisAngle {0}", Passed(BasicRotations[0] == AxisAngleRotations[0]));
            Console.WriteLine("Basic == Quaternion {0}", Passed(BasicRotations[0] == QuaternionRotations[0]));
        }

        private Matrix3 DXMtxToMtx3(DXMtx m)
        {
            return new Matrix3(m.M11, m.M12, m.M13, m.M21, m.M22, m.M23, m.M31, m.M32, m.M33);
        }

        private void TestAdd(Matrix3 a, Matrix3 b, DXMtx dxa, DXMtx dxb)
        {
            AdditionResults = new Matrix3[2];
            AdditionResults[0] = a + b;
            AdditionResults[1] = DXMtxToMtx3(DXMtx.Add(dxa, dxb));
        }

        private void TestSub(Matrix3 a, Matrix3 b, DXMtx dxa, DXMtx dxb)
        {
            SubtractionResults = new Matrix3[2];
            SubtractionResults[0] = a - b;
            SubtractionResults[1] = DXMtxToMtx3(DXMtx.Subtract(dxa, dxb));
        }

        private void TestMul(Matrix3 a, Matrix3 b, DXMtx dxa, DXMtx dxb)
        {
            MultiplyResults = new Matrix3[2];
            MultiplyResults[0] = a * b;
            MultiplyResults[1] = DXMtxToMtx3(DXMtx.Multiply(dxa, dxb));
        }

        private void TestTranspose(Matrix3 a, DXMtx dxa)
        {
            TransposeResults = new Matrix3[2];
            TransposeResults[0] = Matrix3.Transpose(a);
            TransposeResults[1] = DXMtxToMtx3(DXMtx.TransposeMatrix(dxa));
        }

        private void TestInvert(Matrix3 a, DXMtx dxa)
        {
            InverseResults = new Matrix3[2];
            InverseResults[0] = Matrix3.Invert(a);
            InverseResults[1] = DXMtxToMtx3(DXMtx.Invert(dxa));
        }

        private void TestRotations()
        {
            Matrix3 rotx = Matrix3.RotateX(Pitch);
            Matrix3 roty = Matrix3.RotateY(Yaw);
            Matrix3 rotz = Matrix3.RotateZ(Roll);
            DXMtx dxrotx = DXMtx.RotationX(Pitch);
            DXMtx dxroty = DXMtx.RotationY(Yaw);
            DXMtx dxrotz = DXMtx.RotationZ(Roll);

            BasicRotations = new Matrix3[2];
            BasicRotations[0] = rotz * rotx * roty;
            BasicRotations[1] = DXMtxToMtx3(DXMtx.Multiply(DXMtx.Multiply(dxrotz, dxrotx), dxroty));

            EulerRotations = new Matrix3[2];
            EulerRotations[0] = Matrix3.RotateEuler(Yaw, Pitch, Roll);
            EulerRotations[1] = DXMtxToMtx3(DXMtx.RotationYawPitchRoll(Yaw, Pitch, Roll));

            Matrix3.GetAxisAngle(EulerRotations[0], out RotationAxis, out RotationAngle);
            AxisAngleRotations = new Matrix3[2];
            AxisAngleRotations[0] = Matrix3.RotateAxisAngle(RotationAxis, RotationAngle);
            AxisAngleRotations[1] = DXMtxToMtx3(DXMtx.RotationAxis(new Microsoft.DirectX.Vector3(RotationAxis.x, RotationAxis.y, RotationAxis.z), RotationAngle));

            Quaternion quat = new Quaternion(RotationAngle, RotationAxis);
            Microsoft.DirectX.Quaternion dxquat = new Microsoft.DirectX.Quaternion(quat.x, quat.y, quat.z, quat.w);
            QuaternionRotations = new Matrix3[2];
            QuaternionRotations[0] = Matrix3.RotateQuaternion(quat);
            QuaternionRotations[1] = DXMtxToMtx3(DXMtx.RotationQuaternion(dxquat));
        }

        float Yaw, Pitch, Roll;
        Vector3 RotationAxis;
        float RotationAngle;
        Matrix3[] AdditionResults;
        Matrix3[] SubtractionResults;
        Matrix3[] MultiplyResults;
        Matrix3[] TransposeResults;
        Matrix3[] InverseResults;
        Matrix3[] BasicRotations;
        Matrix3[] EulerRotations;
        Matrix3[] AxisAngleRotations;
        Matrix3[] QuaternionRotations;
    }
}
