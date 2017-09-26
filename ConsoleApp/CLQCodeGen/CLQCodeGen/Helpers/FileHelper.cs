using System.IO;

namespace CLQCodeGen.Helpers
{
    public class FileHelper
    {
        public void SaveAndArchiveOriginal(string fileFullName, string fileContent)
        {
            if (File.Exists(fileFullName))
            {
                var archivedFileName = CalculateArchivedFileName(fileFullName);

                File.Move(fileFullName, archivedFileName);
            }

            File.WriteAllText(fileFullName, fileContent);
        }

        private string CalculateArchivedFileName(string fileFullName)
        {
            var archivedFileName = string.Empty;

            if (File.Exists(fileFullName))
            {
                var directoryName = Path.GetDirectoryName(fileFullName);
                var fileName = Path.GetFileNameWithoutExtension(fileFullName);
                var extensionName = Path.GetExtension(fileFullName);

                for (var i = 0;; ++i)
                {
                    var archivedOrder = ".archive" + (i == 0 ? string.Empty : i.ToString());

                    archivedFileName = Path.Combine(directoryName, $"{fileName}{archivedOrder}{extensionName}");

                    if (!File.Exists(archivedFileName))
                    {
                        break;
                    }
                }
            }

            return archivedFileName;
        }
    }
}
