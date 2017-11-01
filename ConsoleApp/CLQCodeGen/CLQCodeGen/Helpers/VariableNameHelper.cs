using System.Collections.Generic;
using System.Text;

namespace CLQCodeGen.Helpers
{
    internal class VariableNameHelper
    {
        public IList<string> SplitIntoWords(string name, bool toLower = false)
        {
            var wordList = new List<string>();
            var wordStrB = new StringBuilder();

            for (int i = 0; i < name.Length; ++i)
            {
                wordStrB.Append(name[i]);

                if ((i + 1) >= name.Length || char.IsUpper(name[i + 1]))
                {
                    wordList.Add(toLower ? wordStrB.ToString().ToLower() : wordStrB.ToString());
                    wordStrB.Clear();
                }
            }

            return wordList;
        }

        public string LowerFirstChar(string name)
        {
            return string.Concat(char.ToLower(name[0]), name.Substring(1));
        }

        public string UpperFirstChar(string name)
        {
            return string.Concat(char.ToUpper(name[0]), name.Substring(1));
        }

        public string ToInFuncName(string name) =>
            name.StartsWith("_") ? name.Substring(1) : name;
    }
}
