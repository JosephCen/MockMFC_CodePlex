//-----------------------------------------------------------------------
// <copyright file="FormAuthenticationAdapter.cs" company="Eurofins">
//  Copyright 2017 Eurofins Scientific Ltd, Ireland
//  Usage reserved to Eurofins Global Franchise Model subscribers.
// </copyright>
//-----------------------------------------------------------------------

using System;
using System.Security.Principal;
using System.Web;
using System.Web.Security;
using JosephCompany.Core.Web.ShareBlock.StatusManagement;

namespace JosephCompany.Core.Web.Security.Authentication
{
    internal class FormAuthenticationAdapter : IFormAuthenticationAdapter
    {
        private readonly IStatus _authenticationSlot;

        public FormAuthenticationAdapter(HttpApplication httpApplication)
            : this(new CookieStatus(FormsAuthentication.FormsCookieName, httpApplication))
        {
        }

        internal FormAuthenticationAdapter(IStatus authenticationSlot)
        {
            _authenticationSlot = authenticationSlot;
        }

        public IPrincipal Principal
        {
            get
            {
                var ticket = DecryptFormsAuthenticationCookie(_authenticationSlot.Value);

                return ticket == null ? null : new GenericPrincipal(new FormsIdentity(ticket), null);
            }

            set
            {
                var now = DateTime.Now;

                _authenticationSlot.Value =
                    EncryptFormsAuthenticationTicket(
                        new FormsAuthenticationTicket(
                            version: 1,
                            name: value.Identity.Name,
                            issueDate: now,
                            expiration: now + FormsAuthentication.Timeout,
                            isPersistent: false,
                            userData: string.Empty));
            }
        }

        public IPrincipal CreatePrincipal(string userName)
        {
            return new GenericPrincipal(new GenericIdentity(userName), new string[] { });
        }

        private static string EncryptFormsAuthenticationTicket(FormsAuthenticationTicket ticket)
        {
            return FormsAuthentication.Encrypt(ticket);
        }

        private static FormsAuthenticationTicket DecryptFormsAuthenticationCookie(string encryptedTicket)
        {
            FormsAuthenticationTicket ticket = null;

            try
            {
                ticket = FormsAuthentication.Decrypt(encryptedTicket);
            }
            catch (ArgumentException ex)
            {
                Console.WriteLine(ex);

                // TODO: Log the exception into log file.
            }

            return ticket;
        }
    }
}
