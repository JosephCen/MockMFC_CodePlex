using System.Collections.Generic;
using System.Text;

namespace CLQCodeGen.Helpers
{
    internal class VariableNameHelper
    {
        public IList<string> SplitIntoWords(string name)
        {
            var wordList = new List<string>();
            var wordStrB = new StringBuilder();

            for (int i = 0; i < name.Length; ++i)
            {
                wordStrB.Append(name[i]);

                if ((i + 1) >= name.Length || char.IsUpper(name[i + 1]))
                {
                    wordList.Add(wordStrB.ToString());
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
    }
}
