import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;

/**
 * 将数据使用DES算法加密,方便进行网络传输,支持中文
 * 
 */
public class Des {

	private final static String ALGORITHM = "DES";
	private final static String PWD = "YOURPASSWORD";

	private final static String ENCODING = "UTF-8";

	public static byte[] encrypt(String data) throws InvalidKeyException, NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, UnsupportedEncodingException, IllegalBlockSizeException, BadPaddingException {
		SecureRandom random = new SecureRandom();

		DESKeySpec desKey = new DESKeySpec(PWD.getBytes());

		// 创建一个密匙工厂，然后用它把DESKeySpec转换成
		SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(ALGORITHM);

		SecretKey securekey = keyFactory.generateSecret(desKey);

		// Cipher对象实际完成加密操作
		Cipher cipher = Cipher.getInstance(ALGORITHM);

		// 用密匙初始化Cipher对象
		cipher.init(Cipher.ENCRYPT_MODE, securekey, random);

		// 现在，获取数据并加密
		// 正式执行加密操作
		byte[] datasource = data.getBytes(ENCODING);
		return cipher.doFinal(datasource);
	}

	public static String decrypt(byte[] src) throws InvalidKeyException, NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, UnsupportedEncodingException, IllegalBlockSizeException, BadPaddingException {
		// DES算法要求有一个可信任的随机数源
		SecureRandom random = new SecureRandom();

		// 创建一个DESKeySpec对象
		DESKeySpec desKey = new DESKeySpec(PWD.getBytes());

		// 创建一个密匙工厂
		SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(ALGORITHM);

		// 将DESKeySpec对象转换成SecretKey对象
		SecretKey securekey = keyFactory.generateSecret(desKey);

		// Cipher对象实际完成解密操作
		Cipher cipher = Cipher.getInstance(ALGORITHM);

		// 用密匙初始化Cipher对象
		cipher.init(Cipher.DECRYPT_MODE, securekey, random);

		// 真正开始解密操作
		return new String(cipher.doFinal(src), ENCODING);
	}
}
