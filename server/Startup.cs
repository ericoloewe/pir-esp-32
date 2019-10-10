using System;
using System.Net;
using System.Net.Sockets;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

namespace server
{
    public class Startup
    {
        private string pirStatus = "No signal...";

        // This method gets called by the runtime. Use this method to add services to the container.
        // For more information on how to configure your application, visit https://go.microsoft.com/fwlink/?LinkID=398940
        public void ConfigureServices(IServiceCollection services)
        {
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseRouting();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapGet("/", async context =>
                {
                    await context.Response.WriteAsync("Hello World!");
                });
            });

            ListenSensorAndBuildWebSocket(app).ContinueWith((task) => Console.WriteLine("Finished"));
        }

        private async Task ListenSensorAndBuildWebSocket(IApplicationBuilder app)
        {
            app.UseWebSockets();

            app.Use(async (context, next) =>
            {
                if (context.Request.Path == "/ws")
                {
                    if (context.WebSockets.IsWebSocketRequest)
                    {
                        WebSocket webSocket = await context.WebSockets.AcceptWebSocketAsync();
                        
                        await Echo(context, webSocket);
                    }
                    else
                    {
                        context.Response.StatusCode = 400;
                    }
                }
                else
                {
                    await next();
                }
            });

            await ListenSocket();
        }

        private async Task Echo(HttpContext context, WebSocket webSocket)
        {
            var buffer = new byte[1024 * 4];
            var result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);

            Console.WriteLine("Start ws");

            while (!result.CloseStatus.HasValue)
            {
                await webSocket.SendAsync(Encoding.UTF8.GetBytes(pirStatus), result.MessageType, result.EndOfMessage, CancellationToken.None);
                Console.WriteLine("Sending ws");

                result = await webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);
            }

            Console.WriteLine("Closed ws");
            await webSocket.CloseAsync(result.CloseStatus.Value, result.CloseStatusDescription, CancellationToken.None);
        }

        private async Task ListenSocket()
        {
            IPAddress ipAddress = IPAddress.Parse("192.168.137.1");
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 9999);
            Socket listener = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            Console.WriteLine("Start to listen");

            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(1000);

                while (true)
                {
                    Socket handler = await listener.AcceptAsync();
                    Console.WriteLine("Start to accept");

                    byte[] bytes = new byte[1024];

                    Console.WriteLine("Socket connected to {0}", localEndPoint.ToString());

                    // Receive the response from the remote device.  
                    int bytesRec = handler.Receive(bytes);

                    pirStatus = Encoding.UTF8.GetString(bytes, 0, bytesRec);
                    Console.WriteLine("Echoed test = {0}", pirStatus);
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                    Thread.Sleep(1000);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
