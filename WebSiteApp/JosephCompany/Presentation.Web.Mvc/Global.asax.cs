//-----------------------------------------------------------------------
// <copyright file="Global.asax.cs" company="Eurofins">
//  Copyright 2017 Eurofins Scientific Ltd, Ireland
//  Usage reserved to Eurofins Global Franchise Model subscribers.
// </copyright>
//-----------------------------------------------------------------------

using System.Web;
using System.Web.Mvc;
using System.Web.Routing;

namespace JosephCompany.Presentation.Web.Mvc
{
    /// <summary>
    /// The application class for current website.
    /// </summary>
    public class MvcApplication : HttpApplication
    {
        /// <summary>
        /// Event happened at application start.
        /// </summary>
        protected void Application_Start()
        {
            AreaRegistration.RegisterAllAreas();
            RouteConfig.RegisterRoutes(RouteTable.Routes);
        }
    }
}
