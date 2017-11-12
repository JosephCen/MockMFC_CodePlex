using System.ComponentModel.DataAnnotations;

namespace Books.Entities
{
    public enum Genere
    {
        [Display(Name = "Non Fiction")]
        NonFiction,
        Romantic,
        ScienceFiction
    }
}