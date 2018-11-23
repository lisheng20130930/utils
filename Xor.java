public class Xor {
	public static int KEY_SIZE = (24);

	public static byte g_key_map[] = { (byte) 0xE3, (byte) 0x82, (byte) 0x49, (byte) 0x42, (byte) 0x39, (byte) 0x32,
			(byte) 0x55, (byte) 0x88, (byte) 0x99, (byte) 0x25, (byte) 0x38, (byte) 0x58, (byte) 0x41, (byte) 0x58,
			(byte) 0x44, (byte) 0x11, (byte) 0x27, (byte) 0x83, (byte) 0x48, (byte) 0x36, (byte) 0x36, (byte) 0x28,
			(byte) 0x33, (byte) 0x22 };

	/*
	 * buffer[] -- The data source to be encoded length -- The bytes number to be encoded
	 */
	public static void encrypt(byte buffer[], int len) {
		for (int i = 0; i < len; i++) {
			buffer[i] -= g_key_map[i % KEY_SIZE];
		}
	}
	
	/*
	 * buffer[] -- The data source to be decoded length -- The bytes number to be decoded
	 */
	public static void decrypt(byte buffer[], int len) {
		for (int i = 0; i < len; i++) {
			buffer[i] += g_key_map[i % KEY_SIZE];
		}
	}
}
