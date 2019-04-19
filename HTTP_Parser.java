import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

/*
 * HTTP 메시지는 CRLF로 각 행을 구분하고
 * 첫번째 행은 특별한 구조를 가지며,
 * 메시지 헤더는 빈 내용의 행이 나오기 전까지 콜론으로 구분된다.
 * GET은 메시지 바디가 없으므로, 메시지 종료시점을 결정하는 데 있어 이런 행 구분자의 위치를 파악하는것이 중요하다캄.
 */
public class Server {

	public static void main(String[] args) throws IOException {
		Server server = new Server();
		server.boot();
	}
	
	private void boot() throws IOException {
		serverSocket = new ServerSocket(8000);
		Socket socket = serverSocket.accept();
		InputStream in = socket.getInputStream();
		
		int oneInt = -1;
		byte oldByte = (byte)-1; //byte는 정수형.
		StringBuilder sb = new StringBuilder();
		int lineNumber = 0;
		
		while(-1 != (oneInt = in.read())) { //한쪽의 socket이 닫히면 -1반환, 웹브라우저의 예로 [x]를 누를때에 해당.
			byte thisByte = (byte)oneInt; //oneInt는 tmp라고할수있다.
			
			//HTTP 메시지를 한 행씩 구분하기 위함
			if(thisByte == Server.LF && oldByte == Server.CR) {
				//CRLF가 완성되었다. 따라서 직전 CRLF부터 여기까지가 한 행이다.
				//-2가 -1을 하는 이유는 아직 LF가 버퍼에 들어가기 전이기 때문이다.
				String oneLine = sb.substring(0, sb.length()-1);
				lineNumber++;
				System.out.printf("%d: %s\n", lineNumber, oneLine);
				if(oneLine.length()<=0) {
					//내용이 없는 행.
					//따라서 메시지 헤더의 마지막일 경우다.
					//향 후 메시지 바디를 처리하며 수정한다.
					break;
				}
			sb.setLength(0); //객체의 길이를 0으로 만들어 stringBuilder를 초기화
			}else {
				sb.append((char)thisByte);
			}
			
			oldByte = (byte)oneInt; //가져온 1byte를 라인피드검사를 위해 잠시 저장해둔다.
		}
		
	}

	public static final byte CR ='\r';
	public static final byte LF ='\n';
	
	private ServerSocket serverSocket;
	}
