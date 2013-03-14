/*
 
 Copyright (c) 2013, SMB Phone Inc.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 
 */
 
#include "Identity.h"
#include <vector>
#include "hookflash/core/IContact.h"

namespace hookflash {
  namespace blackberry {
    namespace app {

      class Contact {
      public:
        Contact(const std::wstring& fullName,
                const std::wstring& profession,
                const std::wstring& avatarUrl,
                Identity::ProvisioningAccountIdentityType idType,
                const std::wstring& identityContactId);

        std::wstring GetFullName() { return mFullName; }
        std::wstring GetProfession() { return mProfession; }
        std::wstring GetAvatarUrl() { return mAvatarUrl; }

        hookflash::core::IContactPtr GetContact() { return mContact; }
        void SetContact(hookflash::core::IContactPtr contact) { mContact = contact; }

        std::wstring GetUserId() { return mUserId; }
        std::wstring GetContactId() { return mContactId; }
        long GetLastProfileUpdateTimestamp() { return mLastProfileUpdateTimestamp; }

        std::vector<Identity> GetIdentities() { return mIdentities; }


      private:

        std::wstring mFullName;
        std::wstring mProfession;
        std::wstring mAvatarUrl;
        hookflash::core::IContactPtr mContact;

        std::wstring mUserId;
        std::wstring mContactId;
        long mLastProfileUpdateTimestamp;

        std::vector<Identity> mIdentities;
      };
    }
  }
}
