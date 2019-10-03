using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;

namespace server
{
    [Route("api/[controller]")]
    public class PirController : Controller
    {
        [HttpGet]
        public string Get()
        {
            return "Ola";
        }
    }
}
