/*
   Copyright 2018 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <OpcUaStackCore/Certificate/CryptoOpenSSLNONE.h>

namespace OpcUaStackCore
{

	CryptoOpenSSLNONE::CryptoOpenSSLNONE(void)
	:CryptoBase()
	{
		securityPolicy("http://opcfoundation.org/UA/SecurityPolicy#None");

		symmetricKeyLen(0);
		minimumAsymmetricKeyLen(0);
		maximumAsymmetricKeyLen(0);
		derivedEncryptionKeyLen(0);
		derivedSignatureKeyLen(0);
		signatureDataLen(0);
		asymmetricSignatureAlgorithmId(0);
		asymmetricEncryptionAlgorithmId(0);
		symmetricSignatureAlgorithmId(0);
		symmetricEncryptionAlgorithmId(0);

	}

	CryptoOpenSSLNONE::~CryptoOpenSSLNONE(void)
	{
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::getAsymmetricEncryptionBlockSize(
		PublicKey& publicKey,
		uint32_t* plainTextBlockSize,
		uint32_t* cryptTextBlockSize
	)
	{
		*plainTextBlockSize = 1;
		*cryptTextBlockSize = 1;
		return Success;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::getAsymmetricSignatureBlockSize(
		PublicKey& publicKey,
		uint32_t* signTextBlockSize
	)
	{
		*signTextBlockSize = 0;
		return Success;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::getSymmetricEncryptionBlockSize(
		uint32_t* plainTextBlockSize,
		uint32_t* cryptTextBlockSize
	)
	{
		*plainTextBlockSize = 1;
		*cryptTextBlockSize = 1;
		return Success;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::getSymmetricSignatureBlockSize(
		uint32_t* signTextBlockSize
	)
	{
		*signTextBlockSize = 0;
		return Success;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::asymmetricKeyLen(
		PublicKey& publicKey,
		uint32_t* asymmetricKeyLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::asymmetricKeyLen(
		PrivateKey& privateKey,
		uint32_t* asymmetricKeyLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::asymmetricDecrypt(
	    char*       	encryptedTextBuf,
		uint32_t		encryptedTextLen,
		PrivateKey&		privateKey,
		char*       	plainTextBuf,
		uint32_t*		plainTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::asymmetricEncrypt(
		char*       	plainTextBuf,
		uint32_t		plainTextLen,
		PublicKey&		publicKey,
		char*       	encryptedTextBuf,
		uint32_t*		encryptedTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::symmetricDecrypt(
		char*       	encryptedTextBuf,
		uint32_t		encryptedTextLen,
		AESKey&	   		aesKey,
		IV&		   		iv,
		char*      		plainTextBuf,
		uint32_t*   	plainTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::symmetricEncrypt(
		char*       	plainTextBuf,
		uint32_t		plainTextLen,
		AESKey&	   		aesKey,
		IV&		   		iv,
		char*      		encryptedTextBuf,
		uint32_t*   	encryptedTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::asymmetricSign(
	    char*       	dataTextBuf,
		uint32_t		dataTextLen,
		PrivateKey&		privateKey,
		char*       	signatureTextBuf,
		uint32_t*		signatureTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::asymmetricVerify(
	    char*       	plainTextBuf,
		uint32_t		plainTextLen,
		PublicKey&		publicKey,
		char*       	signTextBuf,
		uint32_t		signTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::symmetricSign(
	    char*       	dataTextBuf,
		uint32_t		dataTextLen,
		MemoryBuffer&	key,
		char*       	signatureTextBuf,
		uint32_t*		signatureTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::symmetricVerify(
	    char*       	plainTextBuf,
		uint32_t		plainTextLen,
		MemoryBuffer&	key,
		char*       	signTextBuf,
		uint32_t		signTextLen
	)
	{
		return BadNotSupported;
	}

	OpcUaStatusCode
	CryptoOpenSSLNONE::deriveKey(
		MemoryBuffer& secret,			// remote nonce
		MemoryBuffer& seed,				// local nonce
		MemoryBuffer& key				// len = sig key + enc key + iv
	)
	{
		return BadNotSupported;
	}

}
