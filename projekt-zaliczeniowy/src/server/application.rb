require 'socket'
require 'optparse'

def parse_arguments
    options = { port: 1234 }
    OptionParser.new do |opts|
      opts.banner = "Usage: ruby application.rb [options]"

      opts.on("-p <port>", "Select port number") do |p|
        options[:port] = p
      end
    end.parse!
    options
end

options = parse_arguments
server = TCPServer.new options[:port]

puts "[INFO] Listening on port #{ options[:port] }"

loop do
    Thread.start(server.accept) do |client|
        while (line = client.recv(1000)) != ""
            p line
        end
        client.close
        puts "Closing connection"
    end
end
