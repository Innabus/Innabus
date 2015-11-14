using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXQuat = Microsoft.DirectX.Quaternion;
using DXMatrix = Microsoft.DirectX.Matrix;
using DXVec3 = Microsoft.DirectX.Vector3;
using DXVec4 = Microsoft.DirectX.Vector4;

namespace MathRegression
{
    class QuatRegression
    {
        public void RunTests()
        {
            Roll = (float)(45 * Math.PI / 180);
            Yaw = (float)(10 * Math.PI / 180);
            Pitch = (float)(30 * Math.PI / 180);

            TestConversions();
            TestMagnitude();
            TestNormalize();
            TestMul();
            TestInvert();
            TestRotations();
        }

        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }
        
        public void PrintResults()
        {
        }

        private Quaternion DXQToQ(DXQuat q) { return new Quaternion(q.W, q.X, q.Y, q.Z); }

        private void TestConversions()
        {
            Matrix4 eulmtx = Matrix4.RotateEuler(Yaw, Pitch, Roll);
            Vector3 axis;
            float angle;
            Matrix4.GetAxisAngle(eulmtx, out axis, out angle);

            Quaternion eulerq = new Quaternion(Yaw, Pitch, Roll);
            Quaternion axisq = new Quaternion(angle, axis);
            Quaternion compound = (new Quaternion(Roll, new Vector3(0, 0, 1))) * (new Quaternion(Pitch, new Vector3(1, 0, 0))) * (new Quaternion(Yaw, new Vector3(0, 1, 0)));

            DXQuat dxeuler = DXQuat.RotationAxis(new Microsoft.DirectX.Vector3(axis.x, axis.y, axis.z), angle);
            DXQuat dxaxis = DXQuat.RotationYawPitchRoll(Yaw, Pitch, Roll);

            Matrix4 fromeq = eulerq.GetMatrix();
            Matrix4 fromaq = axisq.GetMatrix();
            Matrix4 fromcq = compound.GetMatrix();

            DXMatrix dxcntrlmtx = DXMatrix.RotationYawPitchRoll(Yaw, Pitch, Roll);
            DXMatrix dxeulmtx = DXMatrix.RotationQuaternion(dxeuler);
            DXMatrix dxaxismtx = DXMatrix.RotationQuaternion(dxaxis);
        }

        private void TestMagnitude()
        {
            Quaternion q = new Quaternion(Yaw, Pitch, Roll);
            DXQuat dxq = DXQuat.RotationYawPitchRoll(Yaw, Pitch, Roll);
            MagnitudeResults = new float[2];
            MagnitudeResults[0] = q.Magnitude();
            MagnitudeResults[1] = dxq.Length();
        }

        private void TestNormalize()
        {
            Quaternion q = new Quaternion(1, 2, 3, 4);
            DXQuat dxq = new DXQuat(2, 3, 4, 1);
            q.Normalize();
            dxq.Normalize();
            NormalizeResults = new Quaternion[2];
            NormalizeResults[0] = q;
            NormalizeResults[1] = DXQToQ(dxq);
        }

        private void TestMul()
        {
            float y2 = (float)(30 * Math.PI / 180);
            float r2 = (float)(15 * Math.PI / 180);
            float p2 = (float)(-10 * Math.PI / 180);
            Quaternion a = new Quaternion(Yaw, Pitch, Roll);
            Quaternion b = new Quaternion(y2, p2, r2);
            DXQuat dxa = DXQuat.RotationYawPitchRoll(Yaw, Pitch, Roll);
            DXQuat dxb = DXQuat.RotationYawPitchRoll(y2, p2, r2);
            MulResults = new Quaternion[2];
            MulResults[0] = a * b;
            MulResults[1] = DXQToQ(DXQuat.Multiply(dxa, dxb));
        }

        private void TestInvert()
        {
            Quaternion a = new Quaternion(Yaw, Pitch, Roll);
            Quaternion inv = Quaternion.Invert(a);
            Quaternion result = a * inv;
        }

        private void TestRotations()
        {
            Vector4 v = new Vector4(1, 1, 1, 1);
            Quaternion q = new Quaternion(Yaw, Pitch, Roll);
            Vector4 r = q.RotatePt(v);

            DXVec4 dxv = new DXVec4(1, 1, 1, 1);
            DXQuat dxq = DXQuat.RotationYawPitchRoll(Yaw, Pitch, Roll);
            DXMatrix dxm = DXMatrix.RotationQuaternion(dxq);
            DXVec4 dxr = DXVec4.Transform(dxv, dxm);
        }

        float Yaw, Pitch, Roll;
        float[] MagnitudeResults;
        Quaternion[] NormalizeResults;
        Quaternion[] MulResults;
    }
}
