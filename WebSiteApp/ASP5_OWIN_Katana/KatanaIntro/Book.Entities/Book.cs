using System.ComponentModel.DataAnnotations;

namespace Books.Entities
{
    public class Book
    {
        public int Id { get; set; }

        [Required]
        [StringLength(200)]
        public string Name { get; set; }

        public Genere Catagory { get; set; }
    }
}
