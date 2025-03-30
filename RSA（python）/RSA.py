def rsa_encrypt_decrypt(id):
 print(f"明文为:{id}")
 print("根据第一次实验已知学号反序的最大两个素数为：41122171和41122153")
 print("1.获得两个较大的素数")
 p, q=41122171, 41122153
 print("p,q=41122171,41122153")
 print("2.获得n等于两个素数相乘，并求出n的欧拉函数φ")
 n = p*q
 φ = (p-1)*(q-1)
 print("φ=", φ)
 print("3.选取一个大于1小于φ的素数e")
 e = 29921
 print("e=29921")
 print("4.获取e的模反元素d")
 d = pow(e, -1, φ)
 print("d=", d)
 encrypted_id=pow(id, e, n)
 decrypted_id=pow(encrypted_id, d, n)
 print(f"公钥KU:{e,n}")
 print(f"私钥KR:{d,n}")
 print("密文C为:", encrypted_id)
 print("解出的明文M为:", decrypted_id)
 return encrypted_id, decrypted_id
id = 57122114
encrypted_id, decrypted_id=rsa_encrypt_decrypt(id)