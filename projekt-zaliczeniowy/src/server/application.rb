require 'socket'
require 'optparse'

def parse_arguments
    options = { port: 1234, file: "log", verbose: false }
    OptionParser.new do |opts|
        opts.banner = "Usage: ruby application.rb [options]"

        opts.on("-p <port>", "Select port number") do |p|
            options[:port] = p
        end
        opts.on("-f <file>", "Write logs to file") do |p|
            options[:file] = p
        end
        opts.on("-v", "Print all messages from clients to stdout") do |p|
            options[:verbose] = p
        end
    end.parse!
    options
end

options = parse_arguments
server = TCPServer.new options[:port]
file = File.new options[:file], "w+"

puts "[INFO] Listening on port #{ options[:port] }"

loop do
    Thread.start(server.accept) do |client|
        while (line = client.recv(1000)) != ""
            p line if options[:verbose]
            file.write line
        end
        client.close
        puts "Closing connection"
    end
end
