//-----------------------------------------------------------------------
// <copyright file="CookieStatus.cs" company="Eurofins">
//  Copyright 2017 Eurofins Scientific Ltd, Ireland
//  Usage reserved to Eurofins Global Franchise Model subscribers.
// </copyright>
//-----------------------------------------------------------------------

using System.Web;

namespace JosephCompany.Core.Web.ShareBlock.StatusManagement
{
    /// <summary>
    /// Gets and sets a value with cookie.
    /// </summary>
    public class CookieStatus : IStatus
    {
        private readonly HttpApplication _httpApplication;

        public CookieStatus(string key, HttpApplication httpApplication)
        {
            Key = key;
            _httpApplication = httpApplication;
        }

        public string Key { get; private set; }

        public string Value
        {
            get
            {
                var cookie = _httpApplication.Request.Cookies[Key];

                return cookie == null ? null : cookie.Value;
            }

            set
            {
                _httpApplication.Response.Cookies.Add(new HttpCookie(Key, value));
            }
        }
    }
}
