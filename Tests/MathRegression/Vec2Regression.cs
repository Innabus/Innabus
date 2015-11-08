using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Innabus.Math;
using DXVec2 = Microsoft.DirectX.Vector2;

namespace MathRegression
{
    // Vector -> DXVec2 Function map
    // Magnitude -> Length
    // MagnitudeS -> LengthSq
    // Normalize -> Normalize
    // Add -> Add
    // Sub -> Subtract
    // Mul(f32) -> Multiply
    // Mul(Mtx) -> Transform(Matrix) -- note "Matrix" is always 4x4
    // Dot -> Dot

    class Vec2Regression
    {
        public void RunTests()
        {
            Vector2 v1 = new Vector2(3, 2);
            Vector2 v2 = new Vector2(-4, 5);
            DXVec2 dx1 = new DXVec2(3, 2);
            DXVec2 dx2 = new DXVec2(-4, 5);

            TestMagnitude(v1, v2, dx1, dx2);
            TestNormalize(v1, v2, dx1, dx2);
            TestDot(v1, v2, dx1, dx2);
            TestAdd(v1, v2, dx1, dx2);
            TestSub(v1, v2, dx1, dx2);
            TestScale(v1, v2, dx1, dx2);
            TestTransform(v1, v2, dx1, dx2);

            PrintResults(v1, v2);
        }

        private string Passed(bool b)
        {
            string s = "Passed";
            if (!b)
                s = "Failed";
            return s;
        }

        public void PrintResults(Vector2 v1, Vector2 v2)
        {
            Console.WriteLine("Vector 2 regression tests:");
            Console.WriteLine("A: {0}\nB: {1}", v1, v2);
            Console.WriteLine("Magnitude A: {0} -- {1}", Magnitudes[0], Passed(Magnitudes[0] == Magnitudes[2]));
            Console.WriteLine("Magnitude B: {0} -- {1}", Magnitudes[1], Passed(Magnitudes[1] == Magnitudes[3]));
            Console.WriteLine("MagnitudeSq A: {0} -- {1}", MagnitudesSq[0], Passed(MagnitudesSq[0] == MagnitudesSq[2]));
            Console.WriteLine("MagnitudeSq B: {0} -- {1}", MagnitudesSq[1], Passed(MagnitudesSq[1] == MagnitudesSq[3]));
            Console.WriteLine("Norm A: {0} ({1}) -- {2}", NormResults[0], NormResults[2], Passed(NormResults[0] == NormResults[2]));
            Console.WriteLine("Norm B: {0} ({1}) -- {2}", NormResults[1], NormResults[3], Passed(NormResults[1] == NormResults[3]));
            Console.WriteLine("A Scaled: {0} ({1}) -- {2}", ScaleResults[0], ScaleResults[2], Passed(ScaleResults[0] == ScaleResults[2]));
            Console.WriteLine("B Scaled: {0} ({1}) -- {2}", ScaleResults[1], ScaleResults[3], Passed(ScaleResults[1] == ScaleResults[3]));
            Console.WriteLine("A + B: {0} ({1}) -- {2}", AdditionResults[0], AdditionResults[1], Passed(AdditionResults[0] == AdditionResults[1]));
            Console.WriteLine("A - B: {0} ({1}) -- {2}", SubtractionResults[0], SubtractionResults[1], Passed(SubtractionResults[0] == SubtractionResults[1]));
            Console.WriteLine("A * B: {0} ({1}) -- {2}", DotProducts[0], DotProducts[1], Passed(DotProducts[0] == DotProducts[1]));
            Console.WriteLine("A * M: {0} ({1}) -- {2}", TransformResults[0], TransformResults[2], Passed(TransformResults[0] == TransformResults[2]));
            Console.WriteLine("B * M: {0} ({1}) -- {2}", TransformResults[1], TransformResults[3], Passed(TransformResults[1] == TransformResults[3]));
        }

        private Vector2 DXToV2(DXVec2 dxv)
        {
            return new Vector2(dxv.X, dxv.Y);
        }

        private void TestMagnitude(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            Magnitudes = new float[4];
            Magnitudes[0] = v1.Magnitude();
            Magnitudes[1] = v2.Magnitude();
            Magnitudes[2] = dx1.Length();
            Magnitudes[3] = dx2.Length();

            MagnitudesSq = new float[4];
            MagnitudesSq[0] = v1.MagnitudeS();
            MagnitudesSq[1] = v2.MagnitudeS();
            MagnitudesSq[2] = dx1.LengthSq();
            MagnitudesSq[3] = dx2.LengthSq();
        }

        private void TestNormalize(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            NormResults = new Vector2[4];
            NormResults[0] = (new Vector2(v1)).Normalize();
            NormResults[1] = (new Vector2(v2)).Normalize();
            NormResults[2] = DXToV2(DXVec2.Normalize(dx1));
            NormResults[3] = DXToV2(DXVec2.Normalize(dx2));
        }

        private void TestDot(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            DotProducts = new float[2];
            DotProducts[0] = Vector2.Dot(v1, v2);
            DotProducts[1] = DXVec2.Dot(dx1, dx2);
        }

        private void TestAdd(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            AdditionResults = new Vector2[2];
            AdditionResults[0] = v1 + v2;
            AdditionResults[1] = DXToV2(dx1 + dx2);
        }

        private void TestSub(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            SubtractionResults = new Vector2[2];
            SubtractionResults[0] = v1 - v2;
            SubtractionResults[1] = DXToV2(dx1 - dx2);
        }

        private void TestScale(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            ScaleResults = new Vector2[4];
            ScaleResults[0] = v1 * 2.0f;
            ScaleResults[1] = v2 * .5f;
            ScaleResults[2] = DXToV2(DXVec2.Multiply(dx1, 2.0f));
            ScaleResults[3] = DXToV2(DXVec2.Multiply(dx2, 0.5f));
        }

        private void TestTransform(Vector2 v1, Vector2 v2, DXVec2 dx1, DXVec2 dx2)
        {
            TransformResults = new Vector2[4];
            float rads = (float)(30 * Math.PI / 180);
            Matrix2 rotation = Matrix2.Rotation(rads);
            DXTransformMatrix = Microsoft.DirectX.Matrix.AffineTransformation2D(1, new DXVec2(0, 0), rads, new DXVec2(0, 0));

            DXTransformResults = new Microsoft.DirectX.Vector4[2];
            DXTransformResults[0] = DXVec2.Transform(dx1, DXTransformMatrix);
            DXTransformResults[1] = DXVec2.Transform(dx2, DXTransformMatrix);

            TransformResults[0] = v1 * rotation;
            TransformResults[1] = v2 * rotation;
            TransformResults[2] = new Vector2(DXTransformResults[0].X, DXTransformResults[0].Y);
            TransformResults[3] = new Vector2(DXTransformResults[1].X, DXTransformResults[1].Y);
        }

        float[] Magnitudes;
        float[] MagnitudesSq;
        float[] DotProducts;
        Vector2[] NormResults;
        Vector2[] AdditionResults;
        Vector2[] SubtractionResults;
        Vector2[] ScaleResults;
        Vector2[] TransformResults;
        Microsoft.DirectX.Matrix DXTransformMatrix;
        Microsoft.DirectX.Vector4[] DXTransformResults;
    }
}
