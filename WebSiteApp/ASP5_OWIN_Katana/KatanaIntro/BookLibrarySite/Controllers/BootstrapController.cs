using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace BookLibrarySite.Controllers
{
    public class BootstrapController : Controller
    {
        public ActionResult ButtonGroup()
        {
            return View("ButtonGroup");
        }
    }
}