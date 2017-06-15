//-----------------------------------------------------------------------
// <copyright file="HomeController.cs" company="Eurofins">
//  Copyright 2017 Eurofins Scientific Ltd, Ireland
//  Usage reserved to Eurofins Global Franchise Model subscribers.
// </copyright>
//-----------------------------------------------------------------------

using System.Web.Mvc;

namespace JosephCompany.Presentation.Web.Mvc.Controllers
{
    /// <summary>
    /// The home controller for current site.
    /// </summary>
    public class HomeController : Controller
    {
        /// <summary>
        /// Return index view.
        /// </summary>
        /// <returns>The index view.</returns>
        public ActionResult Index()
        {
            return this.View("Index");
        }
    }
}