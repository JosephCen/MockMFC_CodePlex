//-----------------------------------------------------------------------
// <copyright file="IFormAuthenticationAdapter.cs" company="Eurofins">
//  Copyright 2017 Eurofins Scientific Ltd, Ireland
//  Usage reserved to Eurofins Global Franchise Model subscribers.
// </copyright>
//-----------------------------------------------------------------------

using System.Security.Principal;

namespace JosephCompany.Core.Web.Security.Authentication
{
    /// <summary>
    /// Creates, gets and sets the authentication information for a web form application.
    /// </summary>
    internal interface IFormAuthenticationAdapter
    {
        /// <summary>
        /// Gets or sets principal of current web form request / response.
        /// </summary>
        IPrincipal Principal { get; set; }
        
        /// <summary>
        /// Create a principal instance with given user name.
        /// </summary>
        /// <param name="userName">The user name</param>
        /// <returns>Principal instance.</returns>
        IPrincipal CreatePrincipal(string userName);
    }
}
