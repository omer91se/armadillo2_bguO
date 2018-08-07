import socket
import detect
import json
import pickle


def Main():
    host = ''
    port = 5005

    try:
        s = socket.socket()
        s.bind((host,port))
        s.listen(5)
        print("Server Started.")
        while True:
            conn, addr = s.accept()
            print("client connedted ip:<" + str(addr) + ">")
            filesize= conn.recv(1024)
            print(filesize)
            f = open('image.jpg', 'wb')
            conn.send(b'continue')
            data = conn.recv(1024)
            totalRecv = len(data)
            print(totalRecv)
            f.write(data)
            while float(totalRecv) < float(filesize):
                data = conn.recv(1024)
                totalRecv += len(data)
                f.write(data)
                #print("{0:.2f}".format((totalRecv/float(filesize))*100)+ "% Done")
            print("Download Complete!")
            f.close()
            conn.send(b"done")
            result=detect.detect_center('image.jpg')
            #print(json.dumps(result))
            conn.send(json.dumps(result).encode('utf8'))
            
            conn.close()
            print('ready for more')
            
        s.close()
        
    except KeyboardInterrupt:
        print('^C received, shutting down the web server')
        s.close()
    

if __name__ == '__main__':
    Main()

    
    





	
	
	
