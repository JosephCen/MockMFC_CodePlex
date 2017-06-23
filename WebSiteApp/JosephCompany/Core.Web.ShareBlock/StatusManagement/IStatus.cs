//-----------------------------------------------------------------------
// <copyright file="IStatus.cs" company="Eurofins">
//  Copyright 2017 Eurofins Scientific Ltd, Ireland
//  Usage reserved to Eurofins Global Franchise Model subscribers.
// </copyright>
//-----------------------------------------------------------------------

namespace JosephCompany.Core.Web.ShareBlock.StatusManagement
{
    /// <summary>
    /// Set and get a status in a web application
    /// </summary>
    public interface IStatus
    {
        /// <summary>
        /// Gets the key of current status.
        /// </summary>
        string Key { get; }

        /// <summary>
        /// Gets or sets the value of current status.
        /// </summary>
        string Value { get; set; }
    }
}
