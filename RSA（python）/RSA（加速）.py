def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        g, x, y = extended_gcd(b % a, a)
        return g, y-(b//a)*x, x

def mod_inverse(a, m):
    g, x, y = extended_gcd(a, m)
    if(g != 1):
        raise Exception('Modular inverse does not exist')
    else:
        return x % m

def rsa_encrypt_decrypt_crt(id):
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
    d = mod_inverse(e, φ)
    print("d=", d)
    encrypted_id=pow(id, e, n)
    print("5.使用中国剩余定理进行解密")
    dp = d % (p-1)
    dq = d % (q-1)
    q_inv = mod_inverse(q, p)
    m1 = pow(encrypted_id, dp, p)
    m2 = pow(encrypted_id, dq, q)
    h = (q_inv * (m1-m2)) % p
    decrypted_id = m2 + h * q
    print(f"公钥KU:{e,n}")
    print(f"私钥KR:{d,n}")
    print("密文C为:", encrypted_id)
    print("解出的明文M为:", decrypted_id)
    return encrypted_id, decrypted_id
id = 57122114
encrypted_id, decrypted_id=rsa_encrypt_decrypt_crt(id)
