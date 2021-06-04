/*
 * svc_auth_unix.c
 * Handles UNIX flavor authentication parameters on the service side of rpc.
 * There are two svc auth implementations here: AUTH_UNIX and AUTH_SHORT.
 * _svcauth_unix does full blown unix style uid,gid+gids auth,
 * _svcauth_short uses a shorthand auth to index into a cache of longhand auths.
 * Note: the shorthand has been gutted for efficiency.
 *
 * Copyright (c) 2010, Oracle America, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the "Oracle America, Inc." nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <kernrpc/rpc.h>
#include <kernrpc/svc.h>

#if LW_CFG_NET_RPC_EN > 0

/*
 * Unix longhand authenticator
 */
enum auth_stat
_svcauth_unix (struct svc_req *rqst, struct rpc_msg *msg)
{
  enum auth_stat stat;
  XDR xdrs;
  struct authunix_parms *aup;
  int32_t *buf;
  struct area
    {
      struct authunix_parms area_aup;
      char area_machname[MAX_MACHINE_NAME + 1];
      gid_t area_gids[NGRPS];
    }
   *area;
  u_int auth_len;
  u_int str_len, gid_len;
  u_int i;

  area = (struct area *) rqst->rq_clntcred;
  aup = &area->area_aup;
  aup->aup_machname = area->area_machname;
  aup->aup_gids = area->area_gids;
  auth_len = (u_int) msg->rm_call.cb_cred.oa_length;
  xdrmem_create (&xdrs, msg->rm_call.cb_cred.oa_base, auth_len, XDR_DECODE);
  buf = XDR_INLINE (&xdrs, auth_len);
  if (buf != NULL)
    {
      aup->aup_time = IXDR_GET_LONG (buf);
      str_len = IXDR_GET_U_INT32 (buf);
      if (str_len > MAX_MACHINE_NAME)
	{
	  stat = AUTH_BADCRED;
	  goto done;
	}
      memcpy (aup->aup_machname, (caddr_t) buf, (u_int) str_len);
      aup->aup_machname[str_len] = 0;
      str_len = RNDUP (str_len);
      buf = (int32_t *) ((char *) buf + str_len);
      aup->aup_uid = IXDR_GET_LONG (buf);
      aup->aup_gid = IXDR_GET_LONG (buf);
      gid_len = IXDR_GET_U_INT32 (buf);
      if (gid_len > NGRPS)
	{
	  stat = AUTH_BADCRED;
	  goto done;
	}
      aup->aup_len = gid_len;
      for (i = 0; i < gid_len; i++)
	{
	  aup->aup_gids[i] = IXDR_GET_LONG (buf);
	}
      /*
       * five is the smallest unix credentials structure -
       * timestamp, hostname len (0), uid, gid, and gids len (0).
       */
      if ((5 + gid_len) * BYTES_PER_XDR_UNIT + str_len > auth_len)
	{
	  stat = AUTH_BADCRED;
	  goto done;
	}
    }
  else if (!xdr_authunix_parms (&xdrs, aup))
    {
      xdrs.x_op = XDR_FREE;
      (void) xdr_authunix_parms (&xdrs, aup);
      stat = AUTH_BADCRED;
      goto done;
    }

  /* get the verifier */
  if ((u_int)msg->rm_call.cb_verf.oa_length)
    {
      rqst->rq_xprt->xp_verf.oa_flavor =
	msg->rm_call.cb_verf.oa_flavor;
      rqst->rq_xprt->xp_verf.oa_base =
	msg->rm_call.cb_verf.oa_base;
      rqst->rq_xprt->xp_verf.oa_length =
	msg->rm_call.cb_verf.oa_length;
    }
  else
    {
      rqst->rq_xprt->xp_verf.oa_flavor = AUTH_NULL;
      rqst->rq_xprt->xp_verf.oa_length = 0;
    }
  stat = AUTH_OK;
done:
  XDR_DESTROY (&xdrs);
  return stat;
}


/*
 * Shorthand unix authenticator
 * Looks up longhand in a cache.
 */
/*ARGSUSED */
enum auth_stat
_svcauth_short (struct svc_req *rqst, struct rpc_msg *msg)
{
  return AUTH_REJECTEDCRED;
}

#endif /* LW_CFG_NET_RPC_EN > 0 */
